#ifndef MANDA_NUMBER_HPP
#define MANDA_NUMBER_HPP
#include "object.hpp"

namespace manda::runtime {
class Number : public Object {
public:
  explicit Number(double value);
  double getValue() const;
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;

private:
  double value;
};

class NumberType : public Type {
public:
  std::string getName() const override;
  jit_type_t toJitType() const override;
  std::shared_ptr<Object> applyJitFunction(std::vector<void *> &args,
                                           jit_function &func) override;
};
} // namespace manda::runtime

#endif
