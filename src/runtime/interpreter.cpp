#include "interpreter.hpp"
#include "ansi_printer.hpp"
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

Interpreter::Interpreter(VMOptions options, shared_ptr<Module> &module)
    : options(move(options)), module(module) {
  scopeStack.push(module->getSymbolTable());
}

optional<shared_ptr<Object>> &Interpreter::getLastObject() {
  return lastObject;
}

const VMOptions &Interpreter::getOptions() const { return options; }

void Interpreter::reportError(const manda::analysis::Location &location,
                              const string &message) {
  // TODO: What about thrown errors?
  ostringstream oss;
  oss << "Error: " << location << ": " << message;
  cerr << red(oss.str()) << endl;
}

bool Interpreter::ensureArgumentCount(const Location &location,
                                      const vector<shared_ptr<Object>> &args,
                                      unsigned long size) {
  if (args.size() == size) {
    return true;
  }
  ostringstream oss;
  oss << "Expected " << size << " argument(s), got " << args.size() << ".";
  reportError(location, oss.str());
  return false;
}

bool Interpreter::ensureArguments(const Location &location,
                                  const vector<shared_ptr<Object>> &args,
                                  vector<shared_ptr<Type>> &parameters) {
  // TODO: Add type coercion, etc.
  return false;
}

void Interpreter::visitExprDecl(ExprDeclCtx &ctx) {}

void Interpreter::visitTypeDecl(TypeDeclCtx &ctx) {
  // TODO: Plain mode implementation (lazy?, etc.?)
  TypeResolver resolver(*this, scopeStack.top());
  ctx.type->accept(resolver);
  auto result = resolver.getLastType();
  if (!result) {
    ostringstream oss;
    oss << "Could not resolve the right hand side of type '";
    oss << ctx.name << "'.";
    reportError(ctx.location, oss.str());
    lastObject = nullopt;
  } else {
    // TODO: Handle type parameters for generic types
    // TODO: Handle redefined symbols in non-REPL mode
    scopeStack.top()->add(ctx.name, result, options.isREPL());
  }
}

void Interpreter::visitCompilationUnit(CompilationUnitCtx &ctx) {}

void Interpreter::visitVarExpr(VarExprCtx &ctx) {
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

void Interpreter::visitFnDeclExpr(FnDeclExprCtx &ctx) {
  // TODO: There should probably be more evaluation done here, lol...
  auto value = make_shared<AstFunction>(ctx);
  lastObject = value;
  if (!ctx.name.empty()) {
    // TODO: Handle redefined names
    scopeStack.top()->add(ctx.name, value, options.isREPL());
  }
}

void Interpreter::visitVoidLiteral(VoidLiteralCtx &ctx) {
  lastObject = make_shared<Void>();
}

void Interpreter::visitIdExpr(IdExprCtx &ctx) {
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

void Interpreter::visitNumberLiteral(NumberLiteralCtx &ctx) {
  // TODO: Location
  lastObject = make_shared<Number>(ctx.value);
}

void Interpreter::visitStringLiteral(StringLiteralCtx &ctx) {
  if (ctx.isChar()) {
    lastObject = make_shared<Char>(ctx.getValue()[0]);
  } else {
    lastObject = make_shared<String>(ctx.getValue());
  }
}

void Interpreter::visitBoolLiteral(BoolLiteralCtx &ctx) {
  lastObject = make_shared<Bool>(ctx.value);
}

void Interpreter::visitBlockExpr(BlockExprCtx &ctx) {
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

void Interpreter::visitTupleExpr(TupleExprCtx &ctx) {
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

void Interpreter::visitCastExpr(CastExprCtx &ctx) {}

void Interpreter::visitCallExpr(CallExprCtx &ctx) {
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

void Interpreter::visitParenExpr(ParenExprCtx &ctx) {
  ctx.inner->accept(*this);
}
