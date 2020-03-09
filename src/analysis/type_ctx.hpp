#ifndef MANDA_ANALYSIS_TYPE_HPP
#define MANDA_ANALYSIS_TYPE_HPP
#include "../runtime/type.hpp"
#include "location.hpp"
#include "token.hpp"
#include <string>

namespace manda::analysis {
class TypeVisitor;

class TypeCtx {
public:
  Location location;
  std::shared_ptr<manda::runtime::Type> runtimeType;
  TypeCtx() = default;
  TypeCtx(const Location &location) : location(location) {}
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
  TypeRefCtx(const Location &l, std::string name)
      : name(std::move(name)), TypeCtx(l) {}
  explicit TypeRefCtx(const Token &token)
      : name(token.text), TypeCtx(token.location) {}
  void accept(TypeVisitor &visitor) const override;
  TypeRefCtx *clone() const override;
};

class TypeVisitor {
public:
  virtual void visitTypeRef(const TypeRefCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
