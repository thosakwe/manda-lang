#ifndef MANDA_BOOL_TYPE_HPP
#define MANDA_BOOL_TYPE_HPP
#include "type.hpp"

namespace manda::analysis {
class BoolType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::analysis

#endif
