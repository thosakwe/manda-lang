#include "module_compiler.hpp"
#include "../analysis/ast_printer.hpp"
#include "ast_function.hpp"
#include "number_type.hpp"
#include "type_resolver.hpp"
#include <iostream>
#include <sstream>

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

    // Try to visit functions
    if (auto *fnDecl = dynamic_cast<FnDeclExprCtx *>(topLevel)) {
      visitFnDecl(*fnDecl, scope);
    }

    //    ObjectResolver resolver(interpreter, scope);
    //    topLevel->accept(resolver);
  }
}

void ModuleCompiler::visitTypeDecl(TypeDeclCtx &ctx) {}

void ModuleCompiler::visitFnDecl(FnDeclExprCtx &ctx, UnifiedScope &scope) {
  // Determine the function's return type.
  // If none is given, default to `Any`.
  //
  // TODO: Compare the declared return type to the actual return type.
  shared_ptr<Type> returnType;

  if (!ctx.returnType) {
    TypeResolver typeResolver(interpreter, scope);
    ctx.body->accept(typeResolver);
    returnType = ctx.body->runtimeType;
  } else {
    TypeResolver typeResolver(interpreter, scope);
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
    interpreter.reportError(ctx.location, oss.str());
    lastObject = nullptr;
    // Default to returning Any.
    returnType = interpreter.getCoreLibrary().anyType;
    return;
  }

  // Build the list of parameters.
  // TODO: Handle default symbols on parameters
  vector<Parameter> params;
  if (interpreter.getOptions().developerMode) {
    // TODO: Print param types
    cout << "Total params: " << ctx.params.size() << endl;
  }

  for (auto &node : ctx.params) {
    shared_ptr<Type> type;
    if (!node->type) {
      type = interpreter.getCoreLibrary().anyType;
    } else {
      TypeResolver typeResolver(interpreter, scope);
      node->type->accept(typeResolver);
      type = node->type->runtimeType;
    }

    if (!type) {
      ostringstream oss;
      oss << "Failed to resolve a type for parameter \"";
      oss << node->name << "\".";
      interpreter.reportError(node->location, oss.str());
      lastObject = nullptr;
      return;
    } else {
      if (interpreter.getOptions().developerMode) {
        // TODO: Print param types
        cout << "Found param \"" << node->name << "\"";
      }
      params.push_back({node->location, node->name, type});
    }
  }

  // Build the function object, AND its type.
  auto value = make_shared<AstFunction>(ctx, scope, params, returnType);
  ctx.runtimeType = value->getType(interpreter);
  lastObject = value;

  if (!ctx.name.empty()) {
    // TODO: Handle redefined names
    if (interpreter.getOptions().developerMode) {
      cout << "Defining " << ctx.name << " in current scope." << endl;
    }
    scope.runtimeScope->add(ctx.name, value, interpreter.getOptions().isREPL());
  }
}
