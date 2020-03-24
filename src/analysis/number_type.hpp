#ifndef MANDA_NUMBER_TYPE_HPP
#define MANDA_NUMBER_TYPE_HPP
#include "type.hpp"

namespace manda::analysis {
class NumberType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::analysis

#endif
