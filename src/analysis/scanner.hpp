#ifndef MANDA_SCANNER_HPP
#define MANDA_SCANNER_HPP
#include "token.hpp"
#include <string>
#include <vector>

namespace manda::analysis {
class Scanner {
public:
  explicit Scanner(const std::string &filename, const std::string &sourceText);
  void scan();
  const std::vector<Token> &getTokens() const;

private:
  std::string filename, sourceText;
  std::vector<Token> tokens;
};
} // namespace manda::analysis

#endif
