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

ModuleCompiler::ModuleCompiler(Analyzer &analyzer)
    : analyzer(analyzer) {
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
      module->getTopLevelExpressions().push_back(
          ctx.value->cloneToUniquePointer());
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

void ModuleCompiler::visitFnDecl(FnDeclExprCtx &ctx, const std::shared_ptr<Scope> &scope) {
  // Determine the function's return type.
  // If none is given, default to `Any`.
  //
  // TODO: Compare the declared return type to the actual return type.
  shared_ptr<Type> returnType;

  if (!ctx.returnType) {
    TypeResolver typeResolver(analyzer, scope);
    ctx.body->accept(typeResolver);
    returnType = ctx.body->runtimeType;
  } else {
    TypeResolver typeResolver(analyzer, scope);
    ctx.returnType->accept(typeResolver);
    returnType = ctx.returnType->runtimeType;
  }

  if (!returnType) {
    ostringstream oss;
    oss << "Failed to resolve the return type";
    if (!ctx.name.empty()) {
      oss << " of function \"";
      oss << ctx.name << "\"";
    }
    oss << ".";
    analyzer.errorReporter.reportError(ctx.location, oss.str());
    // Default to returning Any.
    returnType = analyzer.coreLibrary.anyType;
    return;
  }

  // Build the list of parameters.
  // TODO: Handle default symbols on parameters
  vector<Parameter> params;
  if (analyzer.vmOptions.developerMode) {
    // TODO: Print param types
    cout << "Total params: " << ctx.params.size() << endl;
  }

  for (auto &node : ctx.params) {
    shared_ptr<Type> type;
    if (!node->type) {
      type = analyzer.coreLibrary.anyType;
    } else {
      TypeResolver typeResolver(analyzer, scope);
      node->type->accept(typeResolver);
      type = node->type->runtimeType;
    }

    if (!type) {
      ostringstream oss;
      oss << "Failed to resolve a type for parameter \"";
      oss << node->name << "\".";
      analyzer.errorReporter.reportError(node->location, oss.str());
      return;
    } else {
      if (analyzer.vmOptions.developerMode) {
        // TODO: Print param types
        cout << "Found param \"" << node->name << "\"";
      }
      params.push_back({node->location, node->name, type});
    }
  }

  // Build the function object, AND its type.
  auto functionType = make_shared<FunctionType>(params, returnType);
  ctx.runtimeType = functionType;

  if (!ctx.name.empty()) {
    // TODO: Handle redefined names
    if (analyzer.vmOptions.developerMode) {
      cout << "Defining " << ctx.name << " in current scope." << endl;
    }
    scope->add(ctx.name, {ctx.location, functionType}, analyzer.vmOptions.isREPL());
  }
}
