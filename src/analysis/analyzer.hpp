#ifndef MANDA_ANALYZER_HPP
#define MANDA_ANALYZER_HPP
#include "../runtime/error_reporter.hpp"
#include "../runtime/vm_options.hpp"
#include "core_library.hpp"

namespace manda::analysis {
class Analyzer {
public:
  CoreLibrary coreLibrary;
  manda::runtime::ErrorReporter errorReporter;
  manda::runtime::VMOptions vmOptions;

  explicit Analyzer(manda::runtime::VMOptions options);
};
} // namespace manda::analysis

#endif
