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
  Location currentLocation, errorLocation;
  bool hasError;
  std::string filename, sourceText;
  std::vector<Token> tokens;
  void flushErrors();
  void match(std::string pattern, Token::TokenType type,
             const std::string &sourceText, std::vector<Token> &tokens);
  void match_regex(std::string pattern, Token::TokenType type,
                   const std::string &sourceText, std::vector<Token> &tokens);
};
} // namespace manda::analysis

#endif
