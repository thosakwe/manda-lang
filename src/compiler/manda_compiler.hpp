#ifndef MANDA_MANDA_ANALYZER_HPP
#define MANDA_MANDA_ANALYZER_HPP

#include "../analysis/ast.hpp"
#include "../analysis/expr_ctx.hpp"
#include "../runtime/base_resolver.hpp"
#include "../runtime/type_resolver.hpp"

namespace manda::compiler {
/**
 * A class that is capable of resolving the types of every expression
 * in a Manda source file, and producing IR from it.
 */
class MandaCompiler : public analysis::CompilationUnitVisitor,
                      public analysis::ExprVisitor,
                      public analysis::TypeVisitor,
                      public analysis::DeclVisitor,
                      public runtime::BaseResolver {

};
} // namespace manda::compiler

#endif
