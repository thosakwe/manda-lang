#ifndef MANDA_TYPE_HPP
#define MANDA_TYPE_HPP
#include <jit/jit-plus.h>
#include <string>

namespace manda::runtime {
class Type {
public:
  virtual std::string getName() const = 0;
  virtual jit_type_t toJitType() const = 0;
};
} // namespace manda::runtime

#endif
