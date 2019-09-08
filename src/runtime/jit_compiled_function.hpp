#ifndef MANDA_JIT_COMPILED_FUNCTION_HPP
#define MANDA_JIT_COMPILED_FUNCTION_HPP
#include "ast_function.hpp"
#include "symbol_table.hpp"
#include <optional>
#include <stack>

namespace manda::runtime {
class Interpreter;
class JitCompiledFunction : public jit_function,
                            public manda::analysis::ExprVisitor {
public:
  explicit JitCompiledFunction(Interpreter &interpreter, const AstFunction &fn);
  Interpreter &getInterpreter() const;
  jit_value insn_malloc(const jit_value &size);
  jit_value insn_malloc(jit_uint size);
  void visitVarExpr(const analysis::VarExprCtx &ctx) override;
  void visitFnDeclExpr(const analysis::FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(const analysis::VoidLiteralCtx &ctx) override;
  void visitIdExpr(const analysis::IdExprCtx &ctx) override;
  void visitNumberLiteral(const analysis::NumberLiteralCtx &ctx) override;
  void visitStringLiteral(const analysis::StringLiteralCtx &ctx) override;
  void visitBoolLiteral(const analysis::BoolLiteralCtx &ctx) override;
  void visitBlockExpr(const analysis::BlockExprCtx &ctx) override;
  void visitTupleExpr(const analysis::TupleExprCtx &ctx) override;
  void visitCastExpr(const analysis::CastExprCtx &ctx) override;
  void visitCallExpr(const analysis::CallExprCtx &ctx) override;
  void visitParenExpr(const analysis::ParenExprCtx &ctx) override;

protected:
  void build() override;
  jit_type_t create_signature() override;

private:
  Interpreter &interpreter;
  GarbageCollector &gc;
  const AstFunction &astFunction;
  std::optional<jit_value> lastValue;
  std::stack<bool> coerceToAny;
};
} // namespace manda::runtime

#endif
