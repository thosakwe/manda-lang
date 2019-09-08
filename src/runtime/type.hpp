#ifndef MANDA_TYPE_HPP
#define MANDA_TYPE_HPP
#include <jit/jit-plus.h>
#include <string>

namespace manda::runtime {
class Object;
class Type {
public:
  virtual std::string getName() const = 0;
  virtual jit_type_t toJitType() const = 0;
  virtual bool isExactly(const Type &other);
  virtual bool isAssignableTo(const Type &other);
  // TODO: Don't provide a default impl of this, instead let
  // types do the deserialization themselves.
  virtual std::shared_ptr<manda::runtime::Object>
  applyJitFunction(void **args, jit_function &func);
};
} // namespace manda::runtime

#endif
