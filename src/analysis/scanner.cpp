#include "scanner.hpp"
#include <manda_flex_scanner.hpp>

using namespace manda::analysis;
using namespace std;

Scanner::Scanner(const string &filename, const string &contents)
    : filename(filename), contents(contents) {
  mandalex_init(&flexContext);
  location = {filename, 1, 1};
}

Scanner::~Scanner() {
  if (flexContext != nullptr) {
    mandalex_destroy(flexContext);
    flexContext = nullptr;
  }
}

void Scanner::emit(Token::TokenType type) {
  std::string text = mandaget_text(flexContext);
  tokens.push_back({type, location, text});
  location.column += text.length();
}

void Scanner::scan() {
  // TODO: Is this even necessary when using Bison?
}
