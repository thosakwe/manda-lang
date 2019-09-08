#ifndef MANDA_VOID_HPP
#define MANDA_VOID_HPP
#include "object.hpp"

namespace manda::runtime {
class Void : public Object {
public:
  void print(std::ostream &out, bool ansiSupported) const override;
};

class VoidType : public Type {
  std::string getName() const override;
};
} // namespace manda::runtime

#endif
