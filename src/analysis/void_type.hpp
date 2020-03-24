#ifndef MANDA_VOID_TYPE_HPP
#define MANDA_VOID_TYPE_HPP
#include "type.hpp"

namespace manda::analysis {
class VoidType : public Type {
public:
  [[nodiscard]] std::string getName() const override;
};
} // namespace manda::analysis

#endif
