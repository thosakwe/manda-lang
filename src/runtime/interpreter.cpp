#include "interpreter.hpp"
#include "number.hpp"

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

void Interpreter::visitVarExpr(VarExprCtx &ctx) {}

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
