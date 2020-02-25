#ifndef MANDA_JIT_COMPILED_FUNCTION_HPP
#define MANDA_JIT_COMPILED_FUNCTION_HPP
#include "array_type.hpp"
#include "ast_function.hpp"
#include "base_resolver.hpp"
#include "symbol_table.hpp"
#include <optional>
#include <stack>

namespace manda::runtime {
typedef GenericScope<jit_value> JitValueScope;
class Interpreter;
class JitCompiledFunction : public jit_function,
                            public manda::analysis::ExprVisitor,
                            public BaseResolver {
public:
  explicit JitCompiledFunction(Interpreter &interpreter, const AstFunction &fn);
  Interpreter &getInterpreter() const;
  GarbageCollector &getGC();
  jit_value insn_malloc(const jit_value &size);
  jit_value insn_malloc(jit_uint size);
  jit_value insn_gc_ptr_callback(const char *name, const jit_value &ptr,
                                 void *func);
  jit_value insn_gc_incref(const jit_value &ptr);
  jit_value insn_gc_decref(const jit_value &ptr);
  void visitVarExpr(const analysis::VarExprCtx &ctx) override;
  void visitFnDeclExpr(const analysis::FnDeclExprCtx &ctx) override;
  void visitVoidLiteral(const analysis::VoidLiteralCtx &ctx) override;
  void visitIdExpr(const analysis::IdExprCtx &ctx) override;
  void visitNumberLiteral(const analysis::NumberLiteralCtx &ctx) override;
  void visitStringLiteral(const analysis::StringLiteralCtx &ctx) override;
  void visitBoolLiteral(const analysis::BoolLiteralCtx &ctx) override;
  void visitBlockExpr(const analysis::BlockExprCtx &ctx) override;
  void visitTupleExpr(const analysis::TupleExprCtx &ctx) override;
  void visitListExpr(const analysis::ListExprCtx &ctx) override;
  void visitCastExpr(const analysis::CastExprCtx &ctx) override;
  void visitCallExpr(const analysis::CallExprCtx &ctx) override;
  void visitParenExpr(const analysis::ParenExprCtx &ctx) override;
  void visitIfExpr(const analysis::IfExprCtx &ctx) override;

  void visitIfClause(const analysis::IfClauseCtx &ctx, jit_value &output,
                     jit_label &endLabel);

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
