#ifndef MANDA_UNRESOLVED_HPP
#define MANDA_UNRESOLVED_HPP

#include "expr_ctx.hpp"
#include "type.hpp"

namespace manda::analysis {
class UnresolvedType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::analysis

#endif
