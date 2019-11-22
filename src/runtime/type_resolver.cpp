#include "type_resolver.hpp"
#include "function.hpp"
#include "tuple.hpp"
#include <iostream>
#include <sstream>
#include <utility>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

TypeResolver::TypeResolver(Interpreter &interpreter, const UnifiedScope &scope)
    : interpreter(interpreter), BaseResolver(scope) {}

const shared_ptr<Type> &TypeResolver::getLastType() const { return lastType; }

void TypeResolver::visitTypeRef(const TypeRefCtx &ctx) {
  auto symbol = getRuntimeScope()->resolve(ctx.name);
  if (!symbol) {
    ostringstream oss;
    oss << "The name '";
    oss << ctx.name << "' does not exist in this context.";
    interpreter.reportError(ctx.location, oss.str());
    lastType = nullptr;
  } else if (holds_alternative<shared_ptr<Object>>(*symbol)) {
    ostringstream oss;
    oss << "The value of symbol '";
    oss << ctx.name << "' is a value, not a type.";
    interpreter.reportError(ctx.location, oss.str());
    lastType = nullptr;
  } else if (holds_alternative<shared_ptr<Type>>(*symbol)) {
    lastType = get<shared_ptr<Type>>(*symbol);
  } else {
    // TODO: This will probably never be called; but should it throw a error?
    lastType = nullptr;
  }
}

void TypeResolver::visitVarExpr(const VarExprCtx &ctx) {
  // Resolve the type of the expression, and then add to the scope.
  // TODO: What about redefines?
  ctx.value->accept(*this);
  if (!lastType) {
    ostringstream oss;
    oss << "The name \"" << ctx.name << "\" does not exist in this context.";
    interpreter.reportError(ctx.location, oss.str());
  } else {
    if (interpreter.getOptions().developerMode) {
      cout << "TypeResolver found var " << ctx.name << ": "
           << lastType->getName() << endl;
    }
    getCurrentScope().addType(ctx.name, lastType);
  }
}

void TypeResolver::visitFnDeclExpr(const FnDeclExprCtx &ctx) {}

std::shared_ptr<Type> TypeResolver::visitIfClause(const IfClauseCtx &ctx) {
  // Resolve the condition, and make sure it is a bool.
  // TODO: Support Any in bool expression
  ctx.condition->accept(*this);
  if (!lastType) {
    interpreter.reportError(
        ctx.body->location,
        "Could not determine the type of the condition for this if clause.");
    return nullptr;
  } else if (!lastType->isAssignableTo(interpreter.getCoreLibrary().boolType)) {
    ostringstream oss;
    oss << "This expression produces ";
    oss << lastType->getName();
    oss << ", but if conditions can only produce bool.";
    interpreter.reportError(ctx.body->location, oss.str());
  }

  ctx.body->accept(*this);
  return lastType;
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
  return interpreter.getCoreLibrary().anyType;
}

void TypeResolver::visitIfExpr(const IfExprCtx &ctx) {
  // Ensure that the value in the if is a boolean, etc.
  auto ifClauseType = visitIfClause(*ctx.ifClause);
  if (!ifClauseType) {
    // Don't report an additional error here, as visitIfClause will do it.
    lastType = nullptr;
    return;
  }

  // Next, resolve all else-if clauses, and ensure there is a common denominator
  // type.
  // TODO: Reduce to common denominator type
  for (auto &clause : ctx.elseIfClauses) {
    auto clauseType = visitIfClause(*clause);
    if (!clauseType) {
      lastType = nullptr;
      return;
    }
  }

  // If there is no else, the if must return void.
  if (!ctx.elseClause) {
    if (!ifClauseType->isAssignableTo(interpreter.getCoreLibrary().voidType)) {
      interpreter.reportError(ctx.location,
                              "If there is no 'else' clause, then an 'if' "
                              "expression must resolve to the void type.");
      lastType = nullptr;
    }
  } else {
    // TODO: Reduce to common denominator type
  }

  // Return the reduced type.
  lastType = ifClauseType;
}

void TypeResolver::visitVoidLiteral(const VoidLiteralCtx &ctx) {
  lastType = interpreter.getCoreLibrary().voidType;
}

void TypeResolver::visitIdExpr(const IdExprCtx &ctx) {
  // TODO: Probably only use the type stack?
  auto typeSymbol = getTypeScope()->resolve(ctx.name);
  if (typeSymbol) {
    lastType = *typeSymbol;
    return;
  }

  auto symbol = getRuntimeScope()->resolve(ctx.name);
  if (!symbol) {
    ostringstream oss;
    oss << "The name \"" << ctx.name << "\" does not exist in this context.";
    interpreter.reportError(ctx.location, oss.str());
    lastType = nullptr;
  } else if (holds_alternative<shared_ptr<Type>>(*symbol)) {
    // TODO: Reify types?
    ostringstream oss;
    oss << "The symbol \"" << ctx.name
        << "\" resolves to a type, not an object.";
    interpreter.reportError(ctx.location, oss.str());
    lastType = nullptr;
  } else if (holds_alternative<shared_ptr<Object>>(*symbol)) {
    lastType = get<shared_ptr<Object>>(*symbol)->getType(interpreter);
  } else {
    // TODO: Throw an error here, since it should never be reached?
    ostringstream oss;
    oss << "Compiler error on variable \"" << ctx.name
        << "\". Please file a bug report.";
    interpreter.reportError(ctx.location, oss.str());
    lastType = nullptr;
  }
}

void TypeResolver::visitNumberLiteral(const NumberLiteralCtx &ctx) {
  lastType = interpreter.getCoreLibrary().numberType;
}

void TypeResolver::visitStringLiteral(const StringLiteralCtx &ctx) {
  if (ctx.isChar()) {
    lastType = interpreter.getCoreLibrary().charType;
  } else {
    lastType = interpreter.getCoreLibrary().stringType;
  }
}

void TypeResolver::visitBoolLiteral(const BoolLiteralCtx &ctx) {
  lastType = interpreter.getCoreLibrary().boolType;
}

void TypeResolver::visitBlockExpr(const BlockExprCtx &ctx) {
  if (ctx.body.empty()) {
    lastType = interpreter.getCoreLibrary().voidType;
  } else {
    // TODO: Should *all* nodes be visited, or just the last?
    for (auto &node : ctx.body) {
      lastType = nullptr;
      node->accept(*this);
      if (!lastType) {
        interpreter.reportError(
            node->location,
            "Could not resolve the types of all items in this block.");
        return;
      }
    }
  }
}

void TypeResolver::visitTupleExpr(const TupleExprCtx &ctx) {
  vector<shared_ptr<Type>> items;
  for (auto &item : ctx.items) {
    lastType = nullptr;
    item->accept(*this);
    if (!lastType) {
      // TODO: Allow passing as Any
      // TODO: Should any errors be in the TypeResolver at all?
      interpreter.reportError(
          item->location,
          "Could not resolve the types of all items in this tuple.");
      return;
    } else {
      items.push_back(lastType);
    }
  }

  lastType = make_shared<TupleType>(items);
}

void TypeResolver::visitCastExpr(const CastExprCtx &ctx) {}

void TypeResolver::visitCallExpr(const CallExprCtx &ctx) {
  // TODO: Is there a need to resolve parameter types?
  TypeResolver functionTypeResolver(interpreter, getCurrentScope());
  ctx.target->accept(functionTypeResolver);
  auto targetType = functionTypeResolver.getLastType();
  if (!targetType) {
    // TODO: Should an error be thrown here?
    lastType = nullptr;
  } else {
    // See if this is a function.
    auto *functionType = dynamic_cast<FunctionType *>(targetType.get());
    if (!functionType) {
      // TODO: Should an error be thrown when trying to call something other
      // than a function?
      // TODO: What if this is an instantiation of some type?
    } else {
      lastType = functionType->getReturnType();
    }
  }
}

void TypeResolver::visitParenExpr(const ParenExprCtx &ctx) {
  ctx.inner->accept(*this);
}
