#ifndef MANDA_ARRAY_TYPE_HPP
#define MANDA_ARRAY_TYPE_HPP
#include <jit/jit.h>
#include "type.hpp"

namespace manda::runtime {
class ArrayType : public Type {
public:
  struct AbstractData {
    uint64_t length;
    void* data;
  };
  explicit ArrayType(std::shared_ptr<Type> innerType);
  const std::shared_ptr<Type> &getInnerType() const;
  std::string getName() const override;
  jit_type_t toJitType() const override;
  std::shared_ptr<Object> deserialize(manda::runtime::Interpreter &interpreter,
                                      void *ptr) override;
  jit_value boxRawValue(JitCompiledFunction &fn,
                        const jit_value &rawValue) override;

private:
  std::shared_ptr<Type> innerType;
};
} // namespace manda::runtime

#endif
