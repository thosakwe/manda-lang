#ifndef MANDA_ANALYSIS_SCANNER_HPP
#define MANDA_ANALYSIS_SCANNER_HPP
#include "token.hpp"
#include <string>
#include <vector>

#ifndef YY_DECL
#define YY_DECL int manda::analysis::Scanner::mandalex(void *yyscanner)
#endif

namespace manda::analysis {
class Scanner {
public:
  Scanner(const std::string &filename, const std::string &contents);
  Scanner(const Scanner &) = delete;
  Scanner &operator=(const Scanner &) = delete;
  Scanner(Scanner &&) = delete;
  Scanner &operator=(Scanner &&) = delete;
  ~Scanner();
  [[nodiscard]] const std::vector<Token>& getTokens() const;
  void scan();
  int mandalex(void *yyscanner);
  const std::string &getFilename() const;

private:
  std::string filename, contents;
  void *flexContext;
  Location location;
  std::vector<Token> tokens;
  void emit(Token::TokenType type);
};
} // namespace manda::analysis

#endif
