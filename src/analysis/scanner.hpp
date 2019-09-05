#ifndef MANDA_ANALYSIS_SCANNER_HPP
#define MANDA_ANALYSIS_SCANNER_HPP
#include <string>

namespace manda::analysis {
  class Scanner {
    public:
      Scanner(const std::string&);
      Scanner(const Scanner&) = delete;
      Scanner& operator=(const Scanner&) = delete;
      Scanner(Scanner&);
      Scanner& operator=(Scanner&&);
      ~Scanner();
      void scan();
    private:
      void *yyscanner;
  };
}

#endif
