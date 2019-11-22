#ifndef MANDA_ARRAY_HPP
#define MANDA_ARRAY_HPP
#include "object.hpp"
#include <vector>

namespace manda::runtime {
class Array : public Object {
public:
  explicit Array(std::shared_ptr<Type> innerType);
  std::vector<std::shared_ptr<Object>> &getItems();
  [[nodiscard]] const std::shared_ptr<Type> &getInnerType() const;
  [[nodiscard]] const std::vector<std::shared_ptr<Object>> &getItems() const;
  void print(std::ostream &out, bool ansiSupported) const override;
  std::shared_ptr<Type> getType(Interpreter &interpreter) const override;

private:
  std::shared_ptr<Type> innerType;
  std::vector<std::shared_ptr<Object>> items;
};
} // namespace manda::runtime

#endif
