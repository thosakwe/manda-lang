#include "manda_compiler.hpp"

using namespace manda::analysis;
using namespace manda::compiler;
using namespace std;

void MandaCompiler::visitExprDecl(const ExprDeclCtx &ctx) {}

void MandaCompiler::visitTypeDecl(const TypeDeclCtx &ctx) {}

void MandaCompiler::visitCompilationUnit(const CompilationUnitCtx &ctx) {}

void MandaCompiler::visitVarExpr(const VarExprCtx &ctx) {}

void MandaCompiler::visitFnDeclExpr(const FnDeclExprCtx &ctx) {}

void MandaCompiler::visitVoidLiteral(const VoidLiteralCtx &ctx) {}

void MandaCompiler::visitIdExpr(const IdExprCtx &ctx) {}

void MandaCompiler::visitNumberLiteral(const NumberLiteralCtx &ctx) {}

void MandaCompiler::visitStringLiteral(const StringLiteralCtx &ctx) {}

void MandaCompiler::visitBoolLiteral(const BoolLiteralCtx &ctx) {}

void MandaCompiler::visitBlockExpr(const BlockExprCtx &ctx) {}

void MandaCompiler::visitTupleExpr(const TupleExprCtx &ctx) {}

void MandaCompiler::visitCastExpr(const CastExprCtx &ctx) {}

void MandaCompiler::visitCallExpr(const CallExprCtx &ctx) {}

void MandaCompiler::visitParenExpr(const ParenExprCtx &ctx) {}

void MandaCompiler::visitTypeRef(const TypeRefCtx &ctx) {}
