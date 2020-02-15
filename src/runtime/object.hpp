#ifndef MANDA_RUNTIME_OBJECT_HPP
#define MANDA_RUNTIME_OBJECT_HPP
#include "type.hpp"
#include <memory>

namespace manda::runtime {
class Interpreter;
class Object {
public:
  // TODO: Rule of five...
  virtual ~Object() = default;
  virtual void print(std::ostream &out, bool ansiSupported) const = 0;
  virtual std::shared_ptr<Type> getType(Interpreter &interpreter) const = 0;
};
} // namespace manda::runtime

#endif
