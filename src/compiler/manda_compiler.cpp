#include "manda_compiler.hpp"
#include "../ir/ir_function.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace manda::compiler;
using namespace manda::ir;
using namespace std;

const std::shared_ptr<manda::ir::IRModule> &MandaCompiler::getModule() const {
  return module;
}

void MandaCompiler::compile(const CompilationUnitCtx &ctx) {
  // TODO: Module names
  module = make_shared<IRModule>("unnamed");
  visitCompilationUnit(ctx);
}

void MandaCompiler::emit(const IRInstruction &instruction) {
  stateStack.top().block->instructions.push_back(
      make_unique<IRInstruction>(instruction));
}

void MandaCompiler::emit(const manda::ir::IRInstruction::Opcode opcode) {
  IRInstruction instruction{opcode};
  emit(instruction);
}

void MandaCompiler::visitExprDecl(const ExprDeclCtx &ctx) {
  auto *topLevel = dynamic_cast<TopLevelExprCtx *>(ctx.value.get());
  if (!topLevel) {
    // TODO: Proper errors
    cout << "not a top level expr" << endl;
  } else {
    // Is it a function?
    auto *fnDecl = dynamic_cast<FnDeclExprCtx *>(topLevel);
    if (fnDecl) {
      // Create a new IRFunction.
      // TODO: Return types, etc.
      auto func = make_shared<IRFunction>();
      auto block = make_shared<IRBlock>();
      block->name = "entry";
      func->name = fnDecl->name;
      func->blocks.push_back(block);
      stateStack.push({fnDecl, func, block});
      fnDecl->body->accept(*this);
      stateStack.pop();
    }
  }
}

void MandaCompiler::visitTypeDecl(const TypeDeclCtx &ctx) {}

void MandaCompiler::visitCompilationUnit(const CompilationUnitCtx &ctx) {
  // TODO: Forward refs?
  for (auto &decl : ctx.declarations) {
    decl->accept(*this);
  }
}

void MandaCompiler::visitVarExpr(const VarExprCtx &ctx) {}

void MandaCompiler::visitFnDeclExpr(const FnDeclExprCtx &ctx) {
  // TODO: This is only for lambdas
}

void MandaCompiler::visitVoidLiteral(const VoidLiteralCtx &ctx) {}

void MandaCompiler::visitIdExpr(const IdExprCtx &ctx) {}

void MandaCompiler::visitNumberLiteral(const NumberLiteralCtx &ctx) {
  IRInstruction instruction = {IRInstruction::FLOAT_CONST_64};
  instruction.operandf64 = ctx.value;
  emit(instruction);
}

void MandaCompiler::visitStringLiteral(const StringLiteralCtx &ctx) {}

void MandaCompiler::visitBoolLiteral(const BoolLiteralCtx &ctx) {}

void MandaCompiler::visitBlockExpr(const BlockExprCtx &ctx) {}

void MandaCompiler::visitTupleExpr(const TupleExprCtx &ctx) {}

void MandaCompiler::visitCastExpr(const CastExprCtx &ctx) {}

void MandaCompiler::visitCallExpr(const CallExprCtx &ctx) {}

void MandaCompiler::visitParenExpr(const ParenExprCtx &ctx) {}

void MandaCompiler::visitTypeRef(const TypeRefCtx &ctx) {}
