#include "module_compiler.hpp"
#include "../analysis/ast_printer.hpp"
#include "ast_function.hpp"
#include "number.hpp"
#include "object_resolver.hpp"
#include <iostream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

ModuleCompiler::ModuleCompiler(Interpreter &interpreter)
    : interpreter(interpreter) {
  // TODO: Use C++17 fs::path::filename
  module = make_shared<Module>(interpreter.getOptions().inputFile);
  scopeStack.push(module->getSymbolTable());
}

ModuleCompiler::ModuleCompiler(Interpreter &interpreter,
                               std::shared_ptr<Module> &module)
    : interpreter(interpreter), module(module) {
  scopeStack.push(module->getSymbolTable());
}

shared_ptr<Module> &ModuleCompiler::getModule() { return module; }

void ModuleCompiler::visitCompilationUnit(CompilationUnitCtx &ctx) {
  for (auto &node : ctx.declarations) {
    node->accept(*this);
  }
}

void ModuleCompiler::visitExprDecl(ExprDeclCtx &ctx) {
  using namespace manda::analysis;
  auto *topLevel = dynamic_cast<TopLevelExprCtx *>(ctx.value.get());
  if (!topLevel) {
    if (interpreter.getOptions().isREPL()) {
      // TODO: Evaluate top-level expressions
      // IMPORTANT: The value is MOVED.
      module->getTopLevelExpressions().push_back(move(ctx.value));
    } else {
      // TODO: Disallow top-level evaluations in regular mode.
    }
  } else {
    // TODO: Visit top-level
    //    topLevel->accept(*this);
    ObjectResolver resolver(interpreter, module->getSymbolTable());
    topLevel->accept(resolver);
  }
}

void ModuleCompiler::visitTypeDecl(TypeDeclCtx &ctx) {}

void ModuleCompiler::visitVarExpr(VarExprCtx &ctx) {
  // TODO: Protect against redefining names.
  auto &scope = scopeStack.top();
  ctx.value->accept(*this);
  if (lastObject) {
    // TODO: Define in current scope (use a stack)
    scope->add(ctx.name, lastObject.value());
  }
}

void ModuleCompiler::visitFnDeclExpr(FnDeclExprCtx &ctx) {
  // TODO: I can probably delete these...
  // Emit an "empty" function object, that simply points to the fn decl.
  // TODO: There should probably be more evaluation done here, lol...
  // TODO: Pass parameters from ctx to AstFunction
  // TODO: Deduplicate this code
  auto value = make_shared<AstFunction>(ctx);
  auto &scope = scopeStack.top();
  if (!ctx.name.empty()) {
    // TODO: Handle redefined names
    scope->add(ctx.name, value, interpreter.getOptions().isREPL());
  }
}

void ModuleCompiler::visitVoidLiteral(VoidLiteralCtx &ctx) {}

void ModuleCompiler::visitIdExpr(IdExprCtx &ctx) {}

void ModuleCompiler::visitNumberLiteral(NumberLiteralCtx &ctx) {
  // TODO: Track location?
  lastObject = make_shared<Number>(ctx.value);
}

void ModuleCompiler::visitStringLiteral(StringLiteralCtx &ctx) {}

void ModuleCompiler::visitBoolLiteral(BoolLiteralCtx &ctx) {}

void ModuleCompiler::visitBlockExpr(BlockExprCtx &ctx) {}

void ModuleCompiler::visitTupleExpr(TupleExprCtx &ctx) {}

void ModuleCompiler::visitCastExpr(CastExprCtx &ctx) {}

void ModuleCompiler::visitCallExpr(CallExprCtx &ctx) {}

void ModuleCompiler::visitParenExpr(ParenExprCtx &ctx) {}
