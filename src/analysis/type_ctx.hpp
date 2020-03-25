#ifndef MANDA_ANALYSIS_TYPE_HPP
#define MANDA_ANALYSIS_TYPE_HPP
#include "base_ctx.hpp"
#include "location.hpp"
#include "token.hpp"
#include "type.hpp"
#include <string>

namespace manda::analysis {
class TypeVisitor;

class TypeCtx : public BaseCtx {
public:
  std::shared_ptr<manda::analysis::Type> runtimeType;
  TypeCtx() = default;
  explicit TypeCtx(const Location &location) : BaseCtx(location) {}
  TypeCtx(const TypeCtx &) = default;
  TypeCtx(TypeCtx &&) = default;
  TypeCtx &operator=(const TypeCtx &) = default;
  TypeCtx &operator=(TypeCtx &&) = default;
  ~TypeCtx() override = default;
  virtual void accept(TypeVisitor &visitor) = 0;
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
  void accept(TypeVisitor &visitor) override;
  TypeRefCtx *clone() const override;
};

class TypeVisitor {
public:
  virtual void visitTypeRef(TypeRefCtx &ctx) = 0;
};
} // namespace manda::analysis

#endif
