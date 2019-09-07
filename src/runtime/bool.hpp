#ifndef MANDA_BOOL_HPP
#define MANDA_BOOL_HPP
#include "object.hpp"

namespace manda::runtime {
class Bool : public Object {
public:
  explicit Bool(bool value);
  [[nodiscard]] bool getValue() const;
  void print(std::ostream &out, bool ansiSupported) const override;

private:
  bool value;
};
} // namespace manda::runtime

#endif
