#ifndef MANDA_INTERPRETER_HPP
#define MANDA_INTERPRETER_HPP
#include "../analysis/ast.hpp"
#include "base_resolver.hpp"
#include "core_library.hpp"
#include "garbage_collector.hpp"
#include "module.hpp"
#include "symbol_table.hpp"
#include "vm_options.hpp"
#include <jit/jit-plus.h>
#include <optional>
#include <stack>

namespace manda::runtime {
class Interpreter : public manda::analysis::CompilationUnitVisitor,
                    public manda::analysis::DeclVisitor,
                    public BaseResolver {
public:
  Interpreter(VMOptions options, std::shared_ptr<Module> &module);
  std::optional<std::shared_ptr<Object>> &getLastObject();
  [[nodiscard]] const VMOptions &getOptions() const;
  [[nodiscard]] const CoreLibrary &getCoreLibrary() const;
  jit_context &getJitContext();
  GarbageCollector &getGarbageCollector();
  bool ensureArgumentCount(const manda::analysis::Location &location,
                           const std::vector<std::shared_ptr<Object>> &args,
                           unsigned long size);
  bool ensureArguments(const manda::analysis::Location &location,
                       const std::vector<std::shared_ptr<Object>> &args,
                       std::vector<std::shared_ptr<Type>> &parameters);
  void reportError(const manda::analysis::Location &location,
                   const std::string &message);
  void emitTopLevelExpression(std::unique_ptr<manda::analysis::ExprCtx> ctx);
  void visitExprDecl(const analysis::ExprDeclCtx &ctx) override;
  void visitTypeDecl(const analysis::TypeDeclCtx &ctx) override;
  void visitCompilationUnit(const analysis::CompilationUnitCtx &ctx) override;

private:
  CoreLibrary coreLibrary;
  GarbageCollector garbageCollector;
  jit_context jitContext;
  std::shared_ptr<Module> module;
  VMOptions options;
  std::optional<std::shared_ptr<Object>> lastObject;
};
} // namespace manda::runtime

#endif
