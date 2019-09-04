#include "scanner.hpp"

manda::analysis::Scanner::Scanner(const std::string &filename,
                                  const std::string &sourceText)
    : filename(filename), sourceText(sourceText) {
  currentLocation = {filename, 1, 1};
  hasError = false;
}

const std::vector<manda::analysis::Token> &
manda::analysis::Scanner::getTokens() const {
  return tokens;
}

void manda::analysis::Scanner::flushErrors() {
  if (hasError) {
    // TODO: Emit the error.
    hasError = false;
  }
}

void manda::analysis::Scanner::scan() {
  auto text = sourceText;
  std::vector<Token> potential;
  while (!text.empty()) {
    if (text[0] == '\n') {
      text = text.substr(1);
      currentLocation.line++;
      currentLocation.column = 1;
      continue;
    } else if (iswspace(text[0])) {
      text = text.substr(1);
      currentLocation.column++;
      continue;
    }

    match("=", Token::EQUALS, text, potential);
    match("->", Token::EQUALS, text, potential);
    match("<", Token::EQUALS, text, potential);
    match(">", Token::EQUALS, text, potential);
    match("<=", Token::EQUALS, text, potential);
    match(">=", Token::EQUALS, text, potential);
    match("{", Token::EQUALS, text, potential);
    match("}", Token::EQUALS, text, potential);
    match("(", Token::EQUALS, text, potential);
    match(")", Token::EQUALS, text, potential);
    match(":", Token::EQUALS, text, potential);
    match("[", Token::EQUALS, text, potential);
    match("]", Token::EQUALS, text, potential);
    match("..", Token::EQUALS, text, potential);
    match("...", Token::EQUALS, text, potential);
    match(".", Token::EQUALS, text, potential);
    match("+", Token::EQUALS, text, potential);
    match("-", Token::EQUALS, text, potential);
    match("*", Token::EQUALS, text, potential);
    match("/", Token::EQUALS, text, potential);
    match("%", Token::EQUALS, text, potential);
    match("=", Token::EQUALS, text, potential);
    match("^", Token::EQUALS, text, potential);
    match("|", Token::EQUALS, text, potential);
    match("&", Token::EQUALS, text, potential);
    match("||", Token::EQUALS, text, potential);
    match("&&", Token::EQUALS, text, potential);
    match("==", Token::EQUALS, text, potential);
    match(";", Token::EQUALS, text, potential);
    match("as", Token::EQUALS, text, potential);
    match("else", Token::EQUALS, text, potential);
    match("export", Token::EQUALS, text, potential);
    match("for", Token::EQUALS, text, potential);
    match("final", Token::EQUALS, text, potential);
    match("fn", Token::EQUALS, text, potential);
    match("from", Token::EQUALS, text, potential);
    match("if", Token::EQUALS, text, potential);
    match("import", Token::EQUALS, text, potential);
    match("match", Token::EQUALS, text, potential);
    match("pub", Token::EQUALS, text, potential);
    match("sum", Token::EQUALS, text, potential);
    match("then", Token::EQUALS, text, potential);
    match("type", Token::EQUALS, text, potential);
    match("with", Token::TYPE, text, potential);
    match("var", Token::TYPE, text, potential);
    match_regex("[A-Za-z_][A-Za-z0-9_]*", Token::ID, text, potential);
    match_regex("[0-9]+(\\.[0-9]+)?", Token::ID, text, potential);
    if (potential.empty()) {
      if (!hasError) {
        hasError = true;
        errorLocation = currentLocation;
        currentLocation.column++;
      }
      text = text.substr(1);
    } else {
      flushErrors();
      std::sort(potential.begin(), potential.end(), tokenIsLonger);
      auto &token = potential[0];
      tokens.push_back(token);
      currentLocation.column += token.text.length();
      text = text.substr(token.text.length());
    }
  }
}

void manda::analysis::Scanner::match(
    std::string pattern, manda::analysis::Token::TokenType type,
    const std::string &sourceText,
    std::vector<manda::analysis::Token> &tokens) {
  Token token;
  if (sourceText.compare(0, pattern.length(), pattern)) {
    token.type = type;
    token.location = currentLocation;
    token.text = pattern;
    tokens.push_back(token);
  }
}

void manda::analysis::Scanner::match_regex(
    std::string pattern, manda::analysis::Token::TokenType type,
    const std::string &sourceText,
    std::vector<manda::analysis::Token> &tokens) {
  Token token;
  std::regex rgx(pattern);
  if (std::regex_match(sourceText, token.match, rgx)) {
    token.type = type;
    token.location = currentLocation;
    token.text = token.match.str();
    tokens.push_back(token);
  }
}
