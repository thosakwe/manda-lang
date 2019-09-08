#ifndef MANDA_ANALYSIS_TYPE_HPP
#define MANDA_ANALYSIS_TYPE_HPP
#include "location.hpp"
#include <string>

namespace manda::analysis {
class TypeVisitor;

class TypeCtx {
public:
  Location location;
  TypeCtx() = default;
  TypeCtx(const TypeCtx &) = default;
  TypeCtx(TypeCtx &&) = default;
  TypeCtx &operator=(const TypeCtx &) = default;
  TypeCtx &operator=(TypeCtx &&) = default;
  virtual ~TypeCtx() = default;
  virtual void accept(TypeVisitor &visitor) const = 0;
  virtual TypeCtx *clone() const = 0;
  virtual std::unique_ptr<TypeCtx> cloneToUniquePointer() const;
};

struct TypeRefCtx : public TypeCtx {
  std::string name;
  TypeRefCtx() = default;
  TypeRefCtx(const Location &l, std::string name);
  void accept(TypeVisitor &visitor) const override;
  TypeRefCtx *clone() const override;
};

class TypeVisitor {
public:
  virtual void visitTypeRef(const TypeRefCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
