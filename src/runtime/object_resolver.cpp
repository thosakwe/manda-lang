#include "object_resolver.hpp"
#include "any_type.hpp"
#include "ast_function.hpp"
#include "bool.hpp"
#include "char.hpp"
#include "function.hpp"
#include "number.hpp"
#include "string.hpp"
#include "tuple.hpp"
#include "type_resolver.hpp"
#include "void.hpp"
#include <iostream>
#include <sstream>

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
  if (interpreter.getOptions().isREPL()) {
    // Eagerly resolve the value.
    ctx.value->accept(*this);
    if (!lastObject) {
      ostringstream oss;
      oss << "Could not resolve a value for variable '";
      oss << ctx.name << "'.";
      interpreter.reportError(ctx.location, oss.str());
      lastObject = nullptr;
    } else {
      // Add the symbol to the current scope.
      auto symbol = scope->add(ctx.name, lastObject, true);
      // Resolve the value as an identifier in the top-level.
      interpreter.emitTopLevelExpression(
          make_unique<IdExprCtx>(ctx.location, ctx.name));
      lastObject = get<shared_ptr<Object>>(symbol);
    }
  } else {
    lastObject = nullptr;
  }
}

void ObjectResolver::visitFnDeclExpr(FnDeclExprCtx &ctx) {
  // TODO: There should probably be more evaluation done here, lol...
  // TODO: Pass parameters from ctx to AstFunction
  // TODO: Deduplicate this code

  // Build the list of parameters.
  // TODO: Handle default values on parameters
  vector<Parameter> params;
  if (interpreter.getOptions().developerMode) {
    // TODO: Print param types
    cout << "Total params: " << ctx.params.size();
  }
  for (auto &node : ctx.params) {
    shared_ptr<Type> type;
    if (!node->type) {
      type = interpreter.getCoreLibrary().anyType;
    } else {
      TypeResolver typeResolver(interpreter, scope);
      node->type->accept(typeResolver);
      type = typeResolver.getLastType();
    }

    if (!type) {
      ostringstream oss;
      oss << "Failed to resolve a type for parameter \"";
      oss << node->name << "\".";
      interpreter.reportError(node->location, oss.str());
      lastObject = nullptr;
      return;
    } else {
      if (interpreter.getOptions().developerMode) {
        // TODO: Print param types
        cout << "Found param \"" << node->name << "\"";
      }
      params.push_back({node->location, node->name, type});
    }
  }

  auto value = make_shared<AstFunction>(ctx, scope, params);
  lastObject = value;
  if (!ctx.name.empty()) {
    // TODO: Handle redefined names
    scope->add(ctx.name, value, interpreter.getOptions().isREPL());
  }
}

void ObjectResolver::visitVoidLiteral(VoidLiteralCtx &ctx) {
  lastObject = make_shared<Void>();
}

void ObjectResolver::visitIdExpr(IdExprCtx &ctx) {
  auto symbol = scope->resolve(ctx.name);
  if (holds_alternative<monostate>(symbol)) {
    ostringstream oss;
    oss << "The name '";
    oss << ctx.name << "' does not exist in this context.";
    interpreter.reportError(ctx.location, oss.str());
    lastObject = nullptr;
  } else if (holds_alternative<shared_ptr<Type>>(symbol)) {
    ostringstream oss;
    oss << "The value of symbol '";
    oss << ctx.name << "' is a type, not a value.";
    interpreter.reportError(ctx.location, oss.str());
    lastObject = nullptr;
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
  ObjectResolver child(interpreter, scope->createChild());
  for (unsigned long i = 0; i < ctx.body.size(); i++) {
    auto &ptr = ctx.body[i];
    lastObject = nullptr;
    ptr->accept(child);
    if (!lastObject) {
      ostringstream oss;
      oss << "Failed to resolve item " << i;
      oss << " in block.";
      interpreter.reportError(ptr->location, oss.str());
      lastObject = nullptr;
      return;
    }
  }
  if (ctx.body.empty()) {
    lastObject = make_shared<Void>();
  }
}

void ObjectResolver::visitTupleExpr(TupleExprCtx &ctx) {
  auto tup = make_shared<Tuple>();
  for (unsigned long i = 0; i < ctx.items.size(); i++) {
    auto &ptr = ctx.items[i];
    lastObject = nullptr;
    ptr->accept(*this);
    if (!lastObject) {
      ostringstream oss;
      oss << "Failed to resolve item " << i;
      oss << " in tuple.";
      interpreter.reportError(ptr->location, oss.str());
      lastObject = nullptr;
      return;
    } else {
      tup->getItems().push_back(lastObject);
    }
  }
  lastObject = tup;
}

void ObjectResolver::visitCastExpr(CastExprCtx &ctx) {}

void ObjectResolver::visitCallExpr(CallExprCtx &ctx) {
  // Resolve the target first.
  ctx.target->accept(*this);

  if (!lastObject) {
    interpreter.reportError(
        ctx.location, "An error occurred when resolving the call target.");
    lastObject = nullptr;
    return;
  }

  auto *fn = dynamic_cast<Function *>(lastObject.get());
  if (fn == nullptr) {
    interpreter.reportError(ctx.location, "Only functions can be called.");
    lastObject = nullptr;
  } else {
    // Resolve all arguments;
    vector<shared_ptr<Object>> args;
    for (unsigned long i = 0; i < ctx.arguments.size(); i++) {
      auto &ptr = ctx.arguments[i];
      lastObject = nullptr;
      ptr->accept(*this);
      if (!lastObject) {
        ostringstream oss;
        oss << "Failed to resolve item " << i;
        oss << " in call.";
        interpreter.reportError(ptr->location, oss.str());
        lastObject = nullptr;
        return;
      } else {
        args.push_back(lastObject);
      }
    }
    // TODO: This object???
    shared_ptr<Object> thisObject;
    auto result = fn->invoke(interpreter, ctx.location, thisObject, args);
    if (!result) {
      // If nullptr is returned, the function has already set an error.
      lastObject = nullptr;
    } else {
      lastObject = result;
    }
  }
}

void ObjectResolver::visitParenExpr(ParenExprCtx &ctx) {
  ctx.inner->accept(*this);
}
