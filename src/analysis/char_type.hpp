#ifndef MANDA_CHAR_TYPE_HPP
#define MANDA_CHAR_TYPE_HPP
#include "type.hpp"

namespace manda::analysis {
class CharType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::analysis

#endif
