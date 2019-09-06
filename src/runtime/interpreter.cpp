#include "interpreter.hpp"
#include "ansi_printer.hpp"
#include "number.hpp"
#include <iostream>
#include <sstream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Interpreter::Interpreter(VMOptions options, shared_ptr<Module> &module)
    : options(std::move(options)), module(module) {}

std::optional<std::shared_ptr<Object>> &Interpreter::getLastObject() {
  return lastObject;
}

void Interpreter::reportError(const manda::analysis::Location &location,
                              const std::string &message) {
  // TODO: What about thrown errors?
  ostringstream oss;
  oss << "Error: " << location << ": " << message;
  cerr << red(oss.str());
}

void Interpreter::visitExprDecl(ExprDeclCtx &ctx) {}

void Interpreter::visitTypeDecl(TypeDeclCtx &ctx) {}

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

void Interpreter::visitFnDeclExpr(FnDeclExprCtx &ctx) {}

void Interpreter::visitVoidLiteral(VoidLiteralCtx &ctx) {}

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

void Interpreter::visitStringLiteral(StringLiteralCtx &ctx) {}

void Interpreter::visitBoolLiteral(BoolLiteralCtx &ctx) {}

void Interpreter::visitBlockExpr(BlockExprCtx &ctx) {}

void Interpreter::visitTupleExpr(TupleExprCtx &ctx) {}

void Interpreter::visitCastExpr(CastExprCtx &ctx) {}

void Interpreter::visitCallExpr(CallExprCtx &ctx) {}

void Interpreter::visitParenExpr(ParenExprCtx &ctx) {}
