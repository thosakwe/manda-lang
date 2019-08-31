#ifndef MANDA_ANALYSIS_OBJECT_HPP
#define MANDA_ANALYSIS_OBJECT_HPP
#include "type.hpp"
#include <memory>

namespace manda {
namespace analysis {
class Object {
public:
  virtual std::shared_ptr<Type> getType() const;
};
} // namespace analysis
} // namespace manda

#endif
