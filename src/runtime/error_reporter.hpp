#ifndef MANDA_ERROR_REPORTER_HPP
#define MANDA_ERROR_REPORTER_HPP
#include "../analysis/location.hpp"

namespace manda::runtime {
class ErrorReporter {
public:
  void reportError(const manda::analysis::Location &location,
                   const std::string &message);
};
} // namespace manda::runtime

#endif
