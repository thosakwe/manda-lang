#ifndef MANDA_TYPE_HPP
#define MANDA_TYPE_HPP
#include <string>
#include <vector>

namespace manda::runtime {
class Type {
public:
  [[nodiscard]] virtual std::shared_ptr<Type> getParent() const;
  [[nodiscard]] virtual unsigned long getLevel() const;
  virtual std::string getName() const = 0;
  virtual bool isExactly(const Type &other);
  virtual bool isAssignableTo(const Type &other);
  virtual bool isAssignableTo(const std::shared_ptr<Type> &other);
};
} // namespace manda::runtime

#endif
