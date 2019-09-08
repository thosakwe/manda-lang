#ifndef MANDA_JIT_COMPILER_HPP
#define MANDA_JIT_COMPILER_HPP
#include "ast_function.hpp"
#include "symbol_table.hpp"
#include <optional>

namespace manda::runtime {
class Interpreter;
class JitCompiler : public manda::analysis::ExprVisitor {
public:
  explicit JitCompiler(Interpreter& interpreter, const std::shared_ptr<AstFunction> &astFunction);
  jit_function getJitFunction();

private:
  Interpreter& interpreter;
  std::optional<jit_function> jitFunction;
};
} // namespace manda::runtime

#endif
