#ifndef MANDA_SCANNER_HPP
#define MANDA_SCANNER_HPP
#include "token.hpp"
#include <string>
#include <vector>

namespace manda::analysis {
class Scanner {
  explicit Scanner(const std::string &sourceText);
  void scan();
  const std::vector<Token> &getTokens() const;

private:
  std::string sourceText;
  std::vector<Token> tokens;
};
} // namespace manda::analysis

#endif
