#ifndef MANDA_NUMBER_TYPE_HPP
#define MANDA_NUMBER_TYPE_HPP
#include "type.hpp"

namespace manda::runtime {
class NumberType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::runtime

#endif
