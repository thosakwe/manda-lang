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
