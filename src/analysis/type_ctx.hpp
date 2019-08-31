#ifndef MANDA_ANALYSIS_TYPE_HPP
#define MANDA_ANALYSIS_TYPE_HPP
#include <string>

namespace manda::analysis {
class TypeCtx {};

class TypeReferenceCtx : public TypeCtx {
public:
  explicit TypeReferenceCtx(std::string &name);
  const std::string &getName() const;

private:
  std::string name;
};
} // namespace manda::analysis

#endif
