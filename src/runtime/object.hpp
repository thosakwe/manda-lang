#ifndef MANDA_RUNTIME_OBJECT_HPP
#define MANDA_RUNTIME_OBJECT_HPP
#include "type.hpp"
#include <memory>

namespace manda {
namespace runtime {
class Object {
public:
  const std::shared_ptr<Type> &getType() const;

private:
  std::shared_ptr<Type> type;
};
} // namespace runtime
} // namespace manda

#endif