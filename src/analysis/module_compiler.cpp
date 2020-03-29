#include "module_compiler.hpp"
#include "ast_printer.hpp"
#include "function_type.hpp"
#include "number_type.hpp"
#include "type_resolver.hpp"
#include <iostream>
#include <sstream>

using namespace manda::analysis;
using namespace manda::analysis;
using namespace std;

ModuleCompiler::ModuleCompiler(Analyzer &analyzer) : analyzer(analyzer) {
  // TODO: Use C++17 fs::path::filename
  module = make_shared<Module>(analyzer.vmOptions.inputFile);
}

ModuleCompiler::ModuleCompiler(Analyzer &analyzer,
                               std::shared_ptr<Module> &module)
    : analyzer(analyzer), module(module) {}

void ModuleCompiler::visitCompilationUnit(CompilationUnitCtx &ctx) {
  for (auto &node : ctx.declarations) {
    node->accept(*this);
  }
}

void ModuleCompiler::visitExprDecl(ExprDeclCtx &ctx) {
  using namespace manda::analysis;
  auto *topLevel = dynamic_cast<TopLevelExprCtx *>(ctx.value.get());
  if (!topLevel) {
    if (analyzer.vmOptions.isREPL()) {
      // The REPL will evaluate any top-level expressions.
      module->getTopLevelExpressions().push_back(ctx.value);
    } else {
      // Disallow top-level evaluations in regular mode.
      analyzer.errorReporter.reportError(
          ctx.location,
          "This expression is not allowed in the top-level context.");
    }
  } else {
    // Try to visit functions
    if (auto *fnDecl = dynamic_cast<FnDeclExprCtx *>(topLevel)) {
      visitFnDecl(*fnDecl, module->getSymbolTable());
    }

    //    ObjectResolver resolver(interpreter, scope);
    //    topLevel->accept(resolver);
  }
}

void ModuleCompiler::visitTypeDecl(TypeDeclCtx &ctx) {}

void ModuleCompiler::visitFnDecl(FnDeclExprCtx &ctx,
                                 const std::shared_ptr<Scope> &scope) {
  if (!ctx.name.empty()) {
    // TODO: Handle redefined names
    if (analyzer.vmOptions.developerMode) {
      cout << "Defining " << ctx.name << " in current scope." << endl;
    }

    Symbol sym;
    sym.location = ctx.location;
    sym.type = ctx.runtimeType = analyzer.coreLibrary.unresolvedType;
    sym.exprCtx = ctx.shared_from_this();
    scope->add(ctx.name, sym, analyzer.vmOptions.isREPL());
  }
}
