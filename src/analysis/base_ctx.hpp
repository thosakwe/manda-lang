#ifndef MANDA_BASE_CTX_HPP
#define MANDA_BASE_CTX_HPP
#include "location.hpp"

namespace manda::analysis {
class BaseCtx {
public:
  Location location;
  BaseCtx() = default;
  explicit BaseCtx(Location location) : location(std::move(location)) {}
  BaseCtx(const BaseCtx &) = default;
  BaseCtx(BaseCtx &&) = default;
  BaseCtx &operator=(const BaseCtx &) = default;
  BaseCtx &operator=(BaseCtx &&) = default;
  virtual ~BaseCtx() = default;
};
} // namespace manda::analysis

#endif
