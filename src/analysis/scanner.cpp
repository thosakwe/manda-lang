#include "scanner.hpp"
#include <iostream>
#include <manda_flex_scanner.hpp>

using namespace manda::analysis;
using namespace std;

Scanner::Scanner(const string &filename, const string &contents)
    : filename(filename), contents(contents) {
  mandalex_init(&flexContext);
  manda_scan_string(this->contents.c_str(), flexContext);
  location = {filename, 1, 1};
}

const vector<Token> &Scanner::getTokens() const { return tokens; }

Scanner::~Scanner() {
  if (flexContext != nullptr) {
    mandalex_destroy(flexContext);
    flexContext = nullptr;
  }
}

void Scanner::emit(Token::TokenType type) {
  std::string text = mandaget_text(flexContext);
  Token token = {type, location, text};
  tokens.push_back(token);
  //  cout << token << endl;
  location.column += text.length();
}

void Scanner::scan() {
  while (mandalex(flexContext) != 0)
    ;
}
