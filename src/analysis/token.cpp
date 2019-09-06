#include "token.hpp"

std::ostream &manda::analysis::operator<<(std::ostream &out,
                                          const manda::analysis::Token &a) {
  out << a.location << ": \"" << a.text << "\" => " << a.type;
  return out;
}

bool manda::analysis::tokenIsLonger(const Token &a, const Token &b) {
  return a.text.length() < b.text.length();
}
