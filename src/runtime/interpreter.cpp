#include "interpreter.hpp"
#include "number.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Interpreter::Interpreter(VMOptions options, shared_ptr<Module> &module)
    : options(std::move(options)), module(module) {}

std::optional<std::shared_ptr<Object>> &Interpreter::getLastObject() {
  return lastObject;
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
      // TODO: Properly report errors.
      cerr << "error when evaluating var decl" << endl;
    } else {
      // Add the symbol to the current scope.
      auto symbol = scopeStack.top()->add(ctx.name, *lastObject, true);
      // Resolve the value as an identifier in the top-level.
      auto id = make_unique<IdExprCtx>(ctx.location, ctx.name);
      module->getTopLevelExpressions().push_back(move(id));
    }
  } else {
    lastObject = nullopt;
  }
}

void Interpreter::visitFnDeclExpr(FnDeclExprCtx &ctx) {}

void Interpreter::visitVoidLiteral(VoidLiteralCtx &ctx) {}

void Interpreter::visitIdExpr(IdExprCtx &ctx) {}

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
