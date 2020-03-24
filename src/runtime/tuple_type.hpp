#ifndef MANDA_TUPLE_TYPE_HPP
#define MANDA_TUPLE_TYPE_HPP
#include "type.hpp"
#include <vector>

namespace manda::runtime {
class TupleType : public Type {
public:
  explicit TupleType(std::vector<std::shared_ptr<Type>> items);
  [[nodiscard]] std::string getName() const override;

private:
  std::vector<std::shared_ptr<Type>> items;
};
} // namespace manda::runtime

#endif
