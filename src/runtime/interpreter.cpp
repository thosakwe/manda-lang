#include "interpreter.hpp"
#include "ansi_printer.hpp"
#include "ast_function.hpp"
#include "bool.hpp"
#include "char.hpp"
#include "function.hpp"
#include "number.hpp"
#include "string.hpp"
#include "tuple.hpp"
#include "type_resolver.hpp"
#include "void.hpp"
#include <iostream>
#include <sstream>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Interpreter::Interpreter(VMOptions options, shared_ptr<Module> &module)
    : options(move(options)), module(module) {
  coreLibrary.install(*(module->getSymbolTable()));
  scopeStack.push(module->getSymbolTable());
}

optional<shared_ptr<Object>> &Interpreter::getLastObject() {
  return lastObject;
}

const VMOptions &Interpreter::getOptions() const { return options; }

void Interpreter::reportError(const manda::analysis::Location &location,
                              const string &message) {
  // TODO: What about thrown errors?
  ostringstream oss;
  oss << "Error: " << location << ": " << message;
  cerr << red(oss.str()) << endl;
}

void Interpreter::emitTopLevelExpression(
    std::unique_ptr<manda::analysis::ExprCtx> ctx) {
  module->getTopLevelExpressions().push_back(move(ctx));
}

bool Interpreter::ensureArgumentCount(const Location &location,
                                      const vector<shared_ptr<Object>> &args,
                                      unsigned long size) {
  if (args.size() == size) {
    return true;
  }
  ostringstream oss;
  oss << "Expected " << size << " argument(s), got " << args.size() << ".";
  reportError(location, oss.str());
  return false;
}

bool Interpreter::ensureArguments(const Location &location,
                                  const vector<shared_ptr<Object>> &args,
                                  vector<shared_ptr<Type>> &parameters) {
  // TODO: Add type coercion, etc.
  return false;
}

void Interpreter::visitExprDecl(const ExprDeclCtx &ctx) {}

void Interpreter::visitTypeDecl(const TypeDeclCtx &ctx) {
  // TODO: Plain mode implementation (lazy?, etc.?)
  TypeResolver resolver(*this, scopeStack.top());
  ctx.type->accept(resolver);
  auto result = resolver.getLastType();
  if (!result) {
    ostringstream oss;
    oss << "Could not resolve the right hand side of type '";
    oss << ctx.name << "'.";
    reportError(ctx.location, oss.str());
    lastObject = nullopt;
  } else {
    // TODO: Handle type parameters for generic types
    // TODO: Handle redefined symbols in non-REPL mode
    scopeStack.top()->add(ctx.name, result, options.isREPL());
  }
}

void Interpreter::visitCompilationUnit(const CompilationUnitCtx &ctx) {}

const CoreLibrary &Interpreter::getCoreLibrary() const { return coreLibrary; }

jit_context &Interpreter::getJitContext() { return jitContext; }
