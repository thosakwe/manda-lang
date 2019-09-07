#include "object_resolver.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

ObjectResolver::ObjectResolver(Interpreter &interpreter,
                               shared_ptr<SymbolTable> scope)
    : interpreter(interpreter), scope(move(scope)) {}

const shared_ptr<Object> &ObjectResolver::getLastObject() const {
  return lastObject;
}

void ObjectResolver::visitVarExpr(VarExprCtx &ctx) {
  // TODO: Plain mode implementation (lazy?, etc.?)
  if (options.isREPL()) {
    // Eagerly resolve the value.
    ctx.value->accept(*this);
    if (!lastObject) {
      ostringstream oss;
      oss << "Could not resolve a value for variable '";
      oss << ctx.name << "'.";
      reportError(ctx.location, oss.str());
      lastObject = nullopt;
    } else {
      // Add the symbol to the current scope.
      auto symbol = scopeStack.top()->add(ctx.name, *lastObject, true);
      // Resolve the value as an identifier in the top-level.
      auto id = make_unique<IdExprCtx>(ctx.location, ctx.name);
      module->getTopLevelExpressions().push_back(move(id));
      lastObject = get<shared_ptr<Object>>(symbol);
    }
  } else {
    lastObject = nullopt;
  }
}

void ObjectResolver::visitFnDeclExpr(FnDeclExprCtx &ctx) {
  // TODO: There should probably be more evaluation done here, lol...
  // TODO: Pass parameters from ctx to AstFunction
  // TODO: Deduplicate this code
  auto value = make_shared<AstFunction>(ctx);
  lastObject = value;
  if (!ctx.name.empty()) {
    // TODO: Handle redefined names
    scopeStack.top()->add(ctx.name, value, options.isREPL());
  }
}

void ObjectResolver::visitVoidLiteral(VoidLiteralCtx &ctx) {
  lastObject = make_shared<Void>();
}

void ObjectResolver::visitIdExpr(IdExprCtx &ctx) {
  auto symbol = scopeStack.top()->resolve(ctx.name);
  if (holds_alternative<monostate>(symbol)) {
    ostringstream oss;
    oss << "The name '";
    oss << ctx.name << "' does not exist in this context.";
    reportError(ctx.location, oss.str());
    lastObject = nullopt;
  } else if (holds_alternative<shared_ptr<Type>>(symbol)) {
    ostringstream oss;
    oss << "The value of symbol '";
    oss << ctx.name << "' is a type, not a value.";
    reportError(ctx.location, oss.str());
    lastObject = nullopt;
  } else if (holds_alternative<shared_ptr<Object>>(symbol)) {
    lastObject = get<shared_ptr<Object>>(symbol);
  }
}

void ObjectResolver::visitNumberLiteral(NumberLiteralCtx &ctx) {
  // TODO: Location
  lastObject = make_shared<Number>(ctx.value);
}

void ObjectResolver::visitStringLiteral(StringLiteralCtx &ctx) {
  if (ctx.isChar()) {
    lastObject = make_shared<Char>(ctx.getValue()[0]);
  } else {
    lastObject = make_shared<String>(ctx.getValue());
  }
}

void ObjectResolver::visitBoolLiteral(BoolLiteralCtx &ctx) {
  lastObject = make_shared<Bool>(ctx.value);
}

void ObjectResolver::visitBlockExpr(BlockExprCtx &ctx) {
  // TODO: Location
  scopeStack.push(scopeStack.top()->createChild());
  for (unsigned long i = 0; i < ctx.body.size(); i++) {
    auto &ptr = ctx.body[i];
    lastObject = nullopt;
    ptr->accept(*this);
    if (!lastObject) {
      ostringstream oss;
      oss << "Failed to resolve item " << i;
      oss << " in block.";
      reportError(ptr->location, oss.str());
      lastObject = nullopt;
      scopeStack.pop();
      return;
    }
  }
  if (ctx.body.empty()) {
    lastObject = make_shared<Void>();
  }
  scopeStack.pop();
}

void ObjectResolver::visitTupleExpr(TupleExprCtx &ctx) {
  auto tup = make_shared<Tuple>();
  for (unsigned long i = 0; i < ctx.items.size(); i++) {
    auto &ptr = ctx.items[i];
    lastObject = nullopt;
    ptr->accept(*this);
    if (!lastObject) {
      ostringstream oss;
      oss << "Failed to resolve item " << i;
      oss << " in tuple.";
      reportError(ptr->location, oss.str());
      lastObject = nullopt;
      return;
    } else {
      tup->getItems().push_back(*lastObject);
    }
  }
  lastObject = tup;
}

void ObjectResolver::visitCastExpr(CastExprCtx &ctx) {}

void ObjectResolver::visitCallExpr(CallExprCtx &ctx) {
  // Resolve the target first.
  ctx.target->accept(*this);

  if (!lastObject) {
    reportError(ctx.location,
                "An error occurred when resolving the call target.");
    lastObject = nullopt;
    return;
  }

  auto *fn = dynamic_cast<Function *>(lastObject->get());
  if (fn == nullptr) {
    reportError(ctx.location, "Only functions can be called.");
    lastObject = nullopt;
  } else {
    // Resolve all arguments;
    vector<shared_ptr<Object>> args;
    for (unsigned long i = 0; i < ctx.arguments.size(); i++) {
      auto &ptr = ctx.arguments[i];
      lastObject = nullopt;
      ptr->accept(*this);
      if (!lastObject) {
        ostringstream oss;
        oss << "Failed to resolve item " << i;
        oss << " in call.";
        reportError(ptr->location, oss.str());
        lastObject = nullopt;
        return;
      } else {
        args.push_back(*lastObject);
      }
    }
    // TODO: This object???
    shared_ptr<Object> thisObject;
    auto result = fn->invoke(*this, ctx.location, thisObject, args);
    if (!result) {
      // If nullptr is returned, the function has already set an error.
      lastObject = nullopt;
    } else {
      lastObject = result;
    }
  }
}

void ObjectResolver::visitParenExpr(ParenExprCtx &ctx) {
  ctx.inner->accept(*this);
}
