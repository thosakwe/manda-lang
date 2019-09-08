#ifndef MANDA_CHAR_HPP
#define MANDA_CHAR_HPP
#include "object.hpp"

namespace manda::runtime {
class Char : public Object {
public:
  // TODO: Location
  explicit Char(char ch);
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;

private:
  char value;
};

class CharType : public Type {
  std::string getName() const override;

public:
  jit_type_t toJitType() const override;
  std::shared_ptr<manda::runtime::Object> deserialize(void *ptr) override;
  //  std::shared_ptr<manda::runtime::Object>
//  applyJitFunction(std::vector<void *> &args, jit_function &func) override;
};
} // namespace manda::runtime

#endif
