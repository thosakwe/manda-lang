#ifndef MANDA_VOID_HPP
#define MANDA_VOID_HPP
#include "object.hpp"

namespace manda::runtime {
class Void : public Object {
public:
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;
};

class VoidType : public Type {
  std::string getName() const override;

public:
  jit_type_t toJitType() const override;
};
} // namespace manda::runtime

#endif
