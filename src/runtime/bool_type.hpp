#ifndef MANDA_BOOL_TYPE_HPP
#define MANDA_BOOL_TYPE_HPP
#include "type.hpp"

namespace manda::runtime {
class BoolType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::runtime

#endif
