#ifndef MANDA_STRING_HPP
#define MANDA_STRING_HPP
#include "object.hpp"
#include <string>

namespace manda::runtime {
class String : public Object {
public:
  // TODO: Location
  explicit String(std::string s);
  void print(std::ostream &out, bool ansiSupported) const override;

private:
  std::string value;
};
} // namespace manda::runtime

#endif
