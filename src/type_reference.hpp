#ifndef W_TYPE_REFERENCE_HPP
#define W_TYPE_REFERENCE_HPP
#include <string>
#include "type.hpp"

namespace w_lang {
class TypeReference : public Type {
 public:
  explicit TypeReference(std::string name);
  const std::string& getName() const;

 private:
  std::string name;
};
}  // namespace w_lang

#endif

