#ifndef MANDA_RUNTIME_OBJECT_HPP
#define MANDA_RUNTIME_OBJECT_HPP
#include "type.hpp"
#include <memory>

namespace manda::runtime {
class Object {
public:
  virtual const std::shared_ptr<Type> &getType() const = 0;

private:
  std::shared_ptr<Type> type;
};
} // namespace manda::runtime

#endif
