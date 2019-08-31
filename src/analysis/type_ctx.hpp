#ifndef MANDA_ANALYSIS_TYPE_HPP
#define MANDA_ANALYSIS_TYPE_HPP
#include <string>

namespace manda {
namespace analysis {
class TypeCtx {};

class TypeReferenceCtx : public TypeCtx {
public:
  explicit TypeReferenceCtx(std::string &name);
  const std::string &getName() const;

private:
  std::string name;
};
} // namespace analysis
} // namespace manda

#endif
