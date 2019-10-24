#ifndef MANDA_ANALYSIS_SCANNER_HPP
#define MANDA_ANALYSIS_SCANNER_HPP
#include "manda_exception.hpp"
#include "token.hpp"
#include <string>
#include <vector>

#define yyterminate() return TT::END_OF_FILE
#define YY_DECL                                                                \
  manda::analysis::Token::TokenType manda::analysis::Scanner::mandalex(        \
      void *yyscanner)

namespace manda::analysis {
class Scanner : public MandaErrorEmitter {
public:
  Scanner(std::string filename, std::string contents);
  Scanner(const Scanner &) = delete;
  Scanner &operator=(const Scanner &) = delete;
  Scanner(Scanner &&) = delete;
  Scanner &operator=(Scanner &&) = delete;
  ~Scanner();
  [[nodiscard]] const std::string &getFilename() const;
  Token::TokenType mandalex(void *yyscanner);
  Token nextToken();
  [[nodiscard]] bool isDone() const;
  void unexpected(const std::string &text);
  void columns(unsigned long count = 0);
  void lines(unsigned long count = 0);
  void step();

private:
  std::string filename, contents;
  void *buf, *flex;
  bool done;
  Position begin, end;
};
} // namespace manda::analysis

#endif
