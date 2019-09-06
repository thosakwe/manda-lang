#ifndef MANDA_NUMBER_HPP
#define MANDA_NUMBER_HPP
#include "object.hpp"

namespace manda::runtime {
class Number : public Object {
public:
  explicit Number(double value);
  double getValue() const;
  void print(std::ostream &out, bool ansiSupported) const override;

private:
  double value;
};
} // namespace manda::runtime

#endif
