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

    // TODO: String mode
    match("=", Token::EQUALS, text, potential);
    match("<", Token::LT, text, potential);
    match(">", Token::GT, text, potential);
    match("<=", Token::LTE, text, potential);
    match(">=", Token::GTE, text, potential);
    match("{", Token::LCURLY, text, potential);
    match("}", Token::RCURLY, text, potential);
    match("(", Token::LPAREN, text, potential);
    match(")", Token::RPAREN, text, potential);
    match(":", Token::COLON, text, potential);
    match("[", Token::LBRACKET, text, potential);
    match("]", Token::RBRACKET, text, potential);
    match("..", Token::TWO_DOTS, text, potential);
    match("...", Token::THREE_DOTS, text, potential);
    match(".", Token::DOT, text, potential);
    match("+", Token::PLUS, text, potential);
    match("-", Token::MINUS, text, potential);
    match("*", Token::TIMES, text, potential);
    match("/", Token::DIV, text, potential);
    match("%", Token::MOD, text, potential);
    match("^", Token::XOR, text, potential);
    match("|", Token::OR, text, potential);
    match("&", Token::AND, text, potential);
    match("||", Token::BOOL_OR, text, potential);
    match("&&", Token::BOOL_AND, text, potential);
    match("==", Token::BOOL_EQUALS, text, potential);
    match(";", Token::SEMI, text, potential);
    match("as", Token::AS, text, potential);
    match("else", Token::ELSE, text, potential);
    match("export", Token::EXPORT, text, potential);
    match("for", Token::FOR, text, potential);
    match("final", Token::FINAL, text, potential);
    match("fn", Token::FN, text, potential);
    match("from", Token::FROM, text, potential);
    match("if", Token::IF, text, potential);
    match("import", Token::IMPORT, text, potential);
    match("match", Token::MATCH, text, potential);
    match("pub", Token::PUB, text, potential);
    match("sum", Token::SUM, text, potential);
    match("then", Token::THEN, text, potential);
    match("type", Token::TYPE, text, potential);
    match("with", Token::WITH, text, potential);
    match("var", Token::VAR, text, potential);
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
