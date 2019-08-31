#ifndef MANDA_TYPE_REFERENCE_HPP
#define MANDA_TYPE_REFERENCE_HPP
#include <string>
#include "type.hpp"

namespace manda {
class TypeReference : public Type {
 public:
  explicit TypeReference(std::string name);
  const std::string& getName() const;

 private:
  std::string name;
};
}  // namespace manda

#endif

