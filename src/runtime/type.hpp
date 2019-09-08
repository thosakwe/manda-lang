#ifndef MANDA_TYPE_HPP
#define MANDA_TYPE_HPP
#include <string>

namespace manda::runtime {
class Type {
  virtual std::string getName() const = 0;
};
} // namespace manda::runtime

#endif
