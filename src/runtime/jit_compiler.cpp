#include "jit_compiler.hpp"
#include <vector>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

JitCompiler::JitCompiler(Interpreter &interpreter,
                         const shared_ptr<AstFunction> &astFunction)
    : interpreter(interpreter) {
  // Create the JIT function.
  auto signature = astFunction->getType(interpreter)->toJitType();
  jitFunction = jit_function(interpreter.getJitContext(), signature);
}

jit_function JitCompiler::getJitFunction() { return *jitFunction; }
