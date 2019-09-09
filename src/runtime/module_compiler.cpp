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

void ModuleCompiler::visitCompilationUnit(const CompilationUnitCtx &ctx) {
  for (auto &node : ctx.declarations) {
    node->accept(*this);
  }
}

void ModuleCompiler::visitExprDecl(const ExprDeclCtx &ctx) {
  using namespace manda::analysis;
  auto *topLevel = dynamic_cast<TopLevelExprCtx *>(ctx.value.get());
  if (!topLevel) {
    if (interpreter.getOptions().isREPL()) {
      // The REPL will evaluate any top-level expressions.
      module->getTopLevelExpressions().push_back(
          ctx.value->cloneToUniquePointer());
    } else {
      // Disallow top-level evaluations in regular mode.
      interpreter.reportError(
          ctx.location,
          "This expression is not allowed in the top-level context.");
    }
  } else {
    // Visit top-level
    UnifiedScope scope;
    scope.runtimeScope = module->getSymbolTable();
    ObjectResolver resolver(interpreter, scope);
    topLevel->accept(resolver);
  }
}

void ModuleCompiler::visitTypeDecl(const TypeDeclCtx &ctx) {}