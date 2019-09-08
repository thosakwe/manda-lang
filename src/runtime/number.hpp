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
  std::shared_ptr<manda::runtime::Object> deserialize(Interpreter &interpreter,
                                                      void *ptr) override;
  std::shared_ptr<Object> applyJitFunction(Interpreter &interpreter,
                                           std::vector<void *> &args,
                                           jit_function &func) override;
  jit_value boxRawValue(JitCompiledFunction &fn,
                        const jit_value &rawValue) override;
  static Number *box(jit_float64 value);
};
} // namespace manda::runtime

#endif
