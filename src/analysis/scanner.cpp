#include "scanner.hpp"
#include <iostream>
#include <manda_flex_scanner.hpp>

using namespace manda::analysis;
using namespace std;

Scanner::Scanner(const string &filename, const string &contents)
    : filename(filename), contents(contents) {
  mandalex_init(&flex);
  manda_scan_string(this->contents.c_str(), flex);
//  location = {filename, 1, 1};
}

const vector<Token> &Scanner::getTokens() const { return tokens; }

Scanner::~Scanner() {
  if (flex != nullptr) {
    mandalex_destroy(flex);
    flex = nullptr;
  }
}

void Scanner::emit(Token::TokenType type) {
  std::string text = mandaget_text(flex);
  Token token = {type, location, text};
  tokens.push_back(token);
  //  cout << token << endl;
//  location.column += text.length();
}

void Scanner::scan() {
  while (mandalex(flex) != 0)
    ;
}
const string &Scanner::getFilename() const { return filename; }
