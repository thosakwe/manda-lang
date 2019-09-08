#ifndef MANDA_ANY_TYPE_HPP
#define MANDA_ANY_TYPE_HPP
#include "type.hpp"

namespace manda::runtime {
class AnyType : public Type {
  std::string getName() const override;

public:
  jit_type_t toJitType() const override;
  std::shared_ptr<manda::runtime::Object> deserialize(void *ptr) override;
};
} // namespace manda::runtime

#endif
