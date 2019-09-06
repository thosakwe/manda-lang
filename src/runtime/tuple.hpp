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

private:
  std::vector<std::shared_ptr<Object>> items;
};
} // namespace manda::runtime

#endif
