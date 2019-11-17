#ifndef MANDA_ARRAY_TYPE_HPP
#define MANDA_ARRAY_TYPE_HPP
#include "type.hpp"

namespace manda::runtime {
class ArrayType : public Type {
public:
  explicit ArrayType(std::shared_ptr<Type> innerType);
  const std::shared_ptr<Type> &getInnerType() const;
  std::string getName() const override;
  jit_type_t toJitType() const override;

private:
  std::shared_ptr<Type> innerType;
};
} // namespace manda::runtime

#endif
