#ifndef MANDA_BOOL_HPP
#define MANDA_BOOL_HPP
#include "object.hpp"

namespace manda::runtime {
class Bool : public Object {
public:
  explicit Bool(bool value);
  [[nodiscard]] bool getValue() const;
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;

private:
  bool value;
};

class BoolType : public Type {
public:
  std::string getName() const override;
  jit_type_t toJitType() const override;
};
} // namespace manda::runtime

#endif
