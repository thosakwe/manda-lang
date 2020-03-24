#ifndef MANDA_ARRAY_TYPE_HPP
#define MANDA_ARRAY_TYPE_HPP
#include "type.hpp"

namespace manda::analysis {
class ArrayType : public Type {
public:
  explicit ArrayType(std::shared_ptr<Type> innerType);
  [[nodiscard]] const std::shared_ptr<Type> &getInnerType() const;
  [[nodiscard]] std::string getName() const override;
private:
  std::shared_ptr<Type> innerType;
};
} // namespace manda::analysis

#endif
