#ifndef MANDA_ANALYSIS_OBJECT_HPP
#define MANDA_ANALYSIS_OBJECT_HPP
#include "type_ctx.hpp"
#include <memory>

namespace manda::analysis {
class Object {
public:
  virtual std::shared_ptr<TypeCtx> getType() const;
};
} // namespace manda::analysis

#endif
