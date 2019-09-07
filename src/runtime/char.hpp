#ifndef MANDA_CHAR_HPP
#define MANDA_CHAR_HPP
#include "object.hpp"

namespace manda::runtime {
class Char : public Object {
public:
  // TODO: Location
  explicit Char(char ch);
  void print(std::ostream &out, bool ansiSupported) const override;

private:
  char value;
};
} // namespace manda::runtime

#endif
