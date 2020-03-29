#include "type_resolver.hpp"
#include "array_type.hpp"
#include "function_type.hpp"
#include "tuple_type.hpp"
#include <iostream>
#include <sstream>

using namespace manda::analysis;
using namespace std;

TypeResolver::TypeResolver(Analyzer &analyzer,
                           const std::shared_ptr<Scope> &scope)
    : analyzer(analyzer), BaseResolver(scope) {}

void TypeResolver::visitTypeRef(TypeRefCtx &ctx) {
  auto symbol = getCurrentScope()->resolve(ctx.name);
  if (!symbol) {
    ostringstream oss;
    oss << "The name '";
    oss << ctx.name << "' does not exist in this context.";
    analyzer.errorReporter.reportError(ctx.location, oss.str());
    ctx.runtimeType = nullptr;
  } else if (!symbol->typeValue) {
    ostringstream oss;
    oss << "The value of symbol '";
    oss << ctx.name << "' is not a type.";
    analyzer.errorReporter.reportError(ctx.location, oss.str());
    ctx.runtimeType = nullptr;
  } else {
    ctx.runtimeType = symbol->typeValue;
  }
}

void TypeResolver::visitVarExpr(VarExprCtx &ctx) {
  // Resolve the type of the expression, and then add to the scope.
  // TODO: What about redefines?
  ctx.value->accept(*this);
  if (!ctx.value->runtimeType) {
    ostringstream oss;
    oss << "The name \"" << ctx.name << "\" does not exist in this context.";
    analyzer.errorReporter.reportError(ctx.location, oss.str());
  } else {
    if (analyzer.vmOptions.developerMode) {
      cout << "TypeResolver found var " << ctx.name << ": "
           << ctx.value->runtimeType->getName() << endl;
    }
    Symbol sym;
    sym.location = ctx.location;
    sym.type = ctx.value->runtimeType;
    sym.exprCtx = ctx.shared_from_this();
    sym.scope = getCurrentScope();
    getCurrentScope()->add(ctx.name, sym);
  }
}

void TypeResolver::visitFnDeclExpr(FnDeclExprCtx &ctx) {
  // Determine the function's return type.
  // If none is given, default to `Any`.
  //
  // TODO: Compare the declared return type to the actual return type.
  shared_ptr<Type> returnType;

  if (ctx.returnType) {
    // If the function explicitly declares its return type, then trust it.
    TypeResolver typeResolver(analyzer, getCurrentScope());
    ctx.returnType->accept(typeResolver);
    returnType = ctx.returnType->runtimeType;
  }

  // Build the list of parameters.
  // TODO: Handle default symbols on parameters
  vector<Parameter> params;

  for (auto &node : ctx.params) {
    shared_ptr<Type> type;
    if (!node->type) {
      type = analyzer.coreLibrary.anyType;
    } else {
      TypeResolver typeResolver(analyzer, getCurrentScope());
      node->type->accept(typeResolver);
      type = node->type->runtimeType;
    }

    if (!type) {
      ostringstream oss;
      oss << "Failed to resolve a type for parameter \"";
      oss << node->name << "\".";
      analyzer.errorReporter.reportError(node->location, oss.str());
      return;
    } else {
      if (analyzer.vmOptions.developerMode) {
        // TODO: Print param types
        cout << "Found param \"" << node->name << "\"";
      }
      params.push_back({node->location, node->name, type});
    }
  }

  // If the author didn't declare the type, we must eventually
  // manually figure out the return type.
  if (!returnType) {
    // Of course, we must inject parameters into a child scope to try to infer
    // the return type.
    auto childScope = getCurrentScope()->createChild();
    for (auto &param : params) {
      // TODO: Should params just be symbols directly, instead of a separate
      //  class?
      childScope->add(param.name, {param.location, param.type}, true);
    }

    TypeResolver typeResolver(analyzer, childScope);
    ctx.body->accept(typeResolver);
    returnType = ctx.body->runtimeType;
  }

  if (!returnType) {
    ostringstream oss;
    oss << "Failed to resolve the return type";
    if (!ctx.name.empty()) {
      oss << " of function \"";
      oss << ctx.name << "\"";
    }
    oss << ".";
    analyzer.errorReporter.reportError(ctx.location, oss.str());
    // Default to returning Any.
    returnType = analyzer.coreLibrary.anyType;
    return;
  }

  // Build the function object, AND its type.
  auto functionType = make_shared<FunctionType>(params, returnType);
  ctx.runtimeType = functionType;
}

std::shared_ptr<Type> TypeResolver::visitIfClause(IfClauseCtx &ctx) {
  // Resolve the condition, and make sure it is a bool.
  // TODO: Support Any in bool expression
  ctx.condition->accept(*this);
  if (!ctx.condition->runtimeType) {
    analyzer.errorReporter.reportError(
        ctx.body->location,
        "Could not determine the type of the condition for this if clause.");
    return nullptr;
  } else if (!ctx.condition->runtimeType->isAssignableTo(
                 analyzer.coreLibrary.boolType)) {
    ostringstream oss;
    oss << "This expression produces ";
    oss << ctx.condition->runtimeType->getName();
    oss << ", but if conditions can only produce bool.";
    analyzer.errorReporter.reportError(ctx.body->location, oss.str());
  }

  ctx.body->accept(*this);
  return ctx.condition->runtimeType;
}

std::shared_ptr<Type>
TypeResolver::findCommonAncestor(const shared_ptr<Type> &left,
                                 const shared_ptr<Type> &right) {
  // Types are aware of their level within the type tree, so because these can
  // be compared... This becomes an application of the classic "find the
  // intersection of two arrays problem", and therefore is pretty easy to solve.
  auto l = findPathToRoot(left), r = findPathToRoot(right);
  unsigned long i = 0, j = 0;
  while (i < l.size() && j < r.size()) {
    auto &leftType = l[i], &rightType = r[j];
    if (leftType->isExactly(*rightType)) {
      return leftType;
    } else if (leftType->getLevel() < rightType->getLevel()) {
      i++;
    } else {
      j++;
    }
  }
  return analyzer.coreLibrary.anyType;
}

vector<shared_ptr<Type>>
TypeResolver::findPathToRoot(const shared_ptr<Type> &type) {
  vector<shared_ptr<Type>> out;
  auto t = type;
  while (t) {
    out.push_back(t);
    t = t->getParent();
  }
  return out;
}

void TypeResolver::visitIfExpr(IfExprCtx &ctx) {
  // Ensure that the value in the if is a boolean, etc.
  auto ifClauseType = visitIfClause(*ctx.ifClause);
  if (!ifClauseType) {
    // Don't report an additional error here, as visitIfClause will do it.
    ctx.runtimeType = nullptr;
    return;
  }

  // Next, resolve all else-if clauses, and ensure there is a common denominator
  // type.
  for (auto &clause : ctx.elseIfClauses) {
    auto clauseType = visitIfClause(*clause);
    if (!clauseType) {
      ctx.runtimeType = nullptr;
      return;
    } else {
      ctx.runtimeType = findCommonAncestor(ctx.runtimeType, clauseType);
    }
  }

  // If there is no else, the if must return void.
  if (!ctx.elseClause) {
    if (!ifClauseType->isAssignableTo(analyzer.coreLibrary.voidType)) {
      analyzer.errorReporter.reportError(
          ctx.location, "If there is no 'else' clause, then an 'if' "
                        "expression must resolve to the void type.");
      ctx.runtimeType = nullptr;
      return;
    }
  } else {
    // Reduce to common denominator type, once more.
    TypeResolver elseClauseResolver(analyzer, getCurrentScope()->createChild());
    ctx.elseClause->accept(elseClauseResolver);
    if (!ctx.elseClause->runtimeType) {
      analyzer.errorReporter.reportError(
          ctx.elseClause->location,
          "Could not resolve the return type of the 'else' clause, so "
          "resolution of the entire 'if' expression failed.");
      ctx.runtimeType = nullptr;
      return;
    } else {
      ctx.runtimeType =
          findCommonAncestor(ctx.runtimeType, ctx.elseClause->runtimeType);
    }
  }

  // Return the reduced type.
  ctx.runtimeType = ifClauseType;
}

void TypeResolver::visitVoidLiteral(VoidLiteralCtx &ctx) {
  ctx.runtimeType = analyzer.coreLibrary.voidType;
}

void TypeResolver::visitIdExpr(IdExprCtx &ctx) {
  auto symbol = getCurrentScope()->resolve(ctx.name);

  if (!symbol) {
    ostringstream oss;
    oss << "The name \"" << ctx.name << "\" does not exist in this context.";
    analyzer.errorReporter.reportError(ctx.location, oss.str());
    ctx.runtimeType = nullptr;
    return;
  }

  // If this is an unresolved, then fix that.
  if (dynamic_cast<UnresolvedType *>(symbol->type.get())) {
    if (symbol->exprCtx) {
      TypeResolver childResolver(analyzer, symbol->scope);
      symbol->exprCtx->accept(childResolver);
      symbol->type = symbol->exprCtx->runtimeType;
    }
  }

  if (!symbol->typeValue) {
    //    // TODO: Reify types?
    //    ostringstream oss;
    //    oss << "The symbol \"" << ctx.name << "\" does not resolve to a
    //    type."; analyzer.errorReporter.reportError(ctx.location, oss.str());
    //    ctx.runtimeType = nullptr;
    ctx.runtimeType = symbol->type;
  } else {
    symbol->usages.push_back({ctx.location, SymbolUsage::get});
    ctx.runtimeType = symbol->typeValue;
  }
}

void TypeResolver::visitNumberLiteral(NumberLiteralCtx &ctx) {
  ctx.runtimeType = analyzer.coreLibrary.numberType;
}

void TypeResolver::visitStringLiteral(StringLiteralCtx &ctx) {
  if (ctx.isChar()) {
    ctx.runtimeType = analyzer.coreLibrary.charType;
  } else {
    ctx.runtimeType = analyzer.coreLibrary.stringType;
  }
}

void TypeResolver::visitBoolLiteral(BoolLiteralCtx &ctx) {
  ctx.runtimeType = analyzer.coreLibrary.boolType;
}

void TypeResolver::visitBlockExpr(BlockExprCtx &ctx) {
  if (ctx.body.empty()) {
    ctx.runtimeType = analyzer.coreLibrary.voidType;
  } else {
    // TODO: Should *all* nodes be visited, or just the last?
    for (auto &node : ctx.body) {
      ctx.runtimeType = nullptr;
      node->accept(*this);
      if (!ctx.runtimeType) {
        analyzer.errorReporter.reportError(
            node->location,
            "Could not resolve the types of all items in this block.");
        return;
      }
    }
  }
}

void TypeResolver::visitTupleExpr(TupleExprCtx &ctx) {
  vector<shared_ptr<Type>> items;
  for (auto &item : ctx.items) {
    item->accept(*this);
    if (!item->runtimeType) {
      // TODO: Allow passing as Any
      // TODO: Should any errors be in the TypeResolver at all?
      analyzer.errorReporter.reportError(
          item->location,
          "Could not resolve the types of all items in this tuple.");
      return;
    } else {
      items.push_back(item->runtimeType);
    }
  }

  ctx.runtimeType = make_shared<TupleType>(items);
}

void TypeResolver::visitListExpr(ListExprCtx &ctx) {
  shared_ptr<Type> innerType;
  for (auto &item : ctx.items) {
    item->accept(*this);
    if (!item->runtimeType) {
      // TODO: Allow passing as Any
      // TODO: Should any errors be in the TypeResolver at all?
      analyzer.errorReporter.reportError(
          item->location,
          "Could not resolve the types of all items in this list.");
      return;
    } else {
      // Find the nearest parent type, or default to any.
      if (!innerType) {
        innerType = item->runtimeType;
      } else {
        innerType = findCommonAncestor(innerType, item->runtimeType);
      }
    }
  }

  ctx.runtimeType = make_shared<ArrayType>(innerType);
}

void TypeResolver::visitCastExpr(CastExprCtx &ctx) {}

void TypeResolver::visitCallExpr(CallExprCtx &ctx) {
  // TODO: Is there a need to resolve parameter types?
  TypeResolver functionTypeResolver(analyzer, getCurrentScope());
  ctx.target->accept(functionTypeResolver);
  auto targetType = ctx.target->runtimeType;
  if (!targetType) {
    // TODO: Should an error be thrown here?
    ctx.runtimeType = nullptr;
  } else {
    // See if this is a function.
    auto *functionType = dynamic_cast<FunctionType *>(targetType.get());
    if (!functionType) {
      // TODO: Should an error be thrown when trying to call something other
      // than a function?
      // TODO: What if this is an instantiation of some type?
    } else {
      ctx.runtimeType = functionType->getReturnType();
    }
  }
}

void TypeResolver::visitParenExpr(ParenExprCtx &ctx) {
  ctx.inner->accept(*this);
}
