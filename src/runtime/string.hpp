#ifndef MANDA_STRING_HPP
#define MANDA_STRING_HPP
#include "object.hpp"
#include <string>

namespace manda::runtime {
class String : public Object {
public:
  // TODO: Location
  explicit String(std::string s);
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;

private:
  std::string value;
};

class StringType : public Type {
  std::string getName() const override;

public:
  jit_type_t toJitType() const override;
  std::shared_ptr<Object> deserialize(Interpreter &interpreter,
                                      void *ptr) override;
  jit_value boxRawValue(JitCompiledFunction &fn,
                        const jit_value &rawValue) override;

  static String *box(const char *value);
};
} // namespace manda::runtime

#endif
