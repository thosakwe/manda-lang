#ifndef MANDA_TYPE_HPP
#define MANDA_TYPE_HPP
#include <jit/jit-plus.h>
#include <string>
#include <vector>

namespace manda::runtime {
class JitCompiledFunction;
class Interpreter;
class Object;
class Type {
public:
  virtual std::string getName() const = 0;
  virtual jit_type_t toJitType() const = 0;
  virtual bool isExactly(const Type &other);
  virtual bool isAssignableTo(const Type &other);
  virtual bool isAssignableTo(const std::shared_ptr<Type> &other);
  // TODO: Don't provide a default impl of this, instead let
  // types do the deserialization themselves.
  virtual std::shared_ptr<manda::runtime::Object>
  applyJitFunction(Interpreter &interpreter, std::vector<void *> &args,
                   jit_function &func);

  virtual std::shared_ptr<Object>
  deserialize(manda::runtime::Interpreter &interpreter, void *ptr) = 0;

  virtual jit_value boxRawValue(JitCompiledFunction &fn,
                                const jit_value &rawValue);
};
} // namespace manda::runtime

#endif
