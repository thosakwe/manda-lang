#ifndef MANDA_ANALYSIS_SCANNER_HPP
#define MANDA_ANALYSIS_SCANNER_HPP
#include "token.hpp"
#include <string>

#ifndef YY_DECL
#define YY_DECL int manda::analysis::Scanner::yylex(void *yyscanner)
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
  void scan();

private:
  std::string filename, contents;
  void *flex_context;
  int yylex(void *yyscanner);
  void emit(Token::TokenType type);
  void newline();
  void whitespace();
};
} // namespace manda::analysis

#endif
