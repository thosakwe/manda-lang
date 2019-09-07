#ifndef MANDA_INTERPRETER_HPP
#define MANDA_INTERPRETER_HPP
#include "../analysis/ast.hpp"
#include "module.hpp"
#include "symbol_table.hpp"
#include "vm_options.hpp"
#include <optional>
#include <stack>

namespace manda::runtime {
class Interpreter : public manda::analysis::CompilationUnitVisitor,
                    public manda::analysis::DeclVisitor {
public:
  Interpreter(VMOptions options, std::shared_ptr<Module> &module);
  std::optional<std::shared_ptr<Object>> &getLastObject();
  [[nodiscard]] const VMOptions &getOptions() const;
  bool ensureArgumentCount(const manda::analysis::Location &location,
                           const std::vector<std::shared_ptr<Object>> &args,
                           unsigned long size);
  bool ensureArguments(const manda::analysis::Location &location,
                       const std::vector<std::shared_ptr<Object>> &args,
                       std::vector<std::shared_ptr<Type>> &parameters);
  void reportError(const manda::analysis::Location &location,
                   const std::string &message);
  void emitTopLevelExpression(std::unique_ptr<manda::analysis::ExprCtx> ctx);
  void visitExprDecl(analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(analysis::TypeDeclCtx &ctx) override;
  void visitCompilationUnit(analysis::CompilationUnitCtx &ctx) override;

private:
  std::shared_ptr<Module> module;
  VMOptions options;
  std::optional<std::shared_ptr<Object>> lastObject;
  std::stack<std::shared_ptr<SymbolTable>> scopeStack;
};
} // namespace manda::runtime

#endif
