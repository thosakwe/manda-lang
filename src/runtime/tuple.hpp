#ifndef MANDA_TUPLE_HPP
#define MANDA_TUPLE_HPP
#include "object.hpp"
#include <vector>

namespace manda::runtime {
class Tuple : public Object {
public:
  // TODO: Location
  std::vector<std::shared_ptr<Object>> &getItems();
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;

private:
  std::vector<std::shared_ptr<Object>> items;
};

class TupleType : public Type {
public:
  explicit TupleType(std::vector<std::shared_ptr<Type>> items);
  jit_type_t toJitType() const override;
  std::shared_ptr<manda::runtime::Object> deserialize(void *ptr) override;

private:
  std::string getName() const override;

private:
  std::vector<std::shared_ptr<Type>> items;
};
} // namespace manda::runtime

#endif
