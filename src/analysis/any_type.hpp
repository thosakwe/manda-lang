#ifndef MANDA_ANY_TYPE_HPP
#define MANDA_ANY_TYPE_HPP
#include "type.hpp"

namespace manda::analysis {
class AnyType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::analysis

#endif
