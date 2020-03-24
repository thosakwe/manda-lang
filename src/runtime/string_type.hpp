#ifndef MANDA_STRING_TYPE_HPP
#define MANDA_STRING_TYPE_HPP
#include "type.hpp"
#include <string>

namespace manda::runtime {
class StringType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::runtime

#endif
