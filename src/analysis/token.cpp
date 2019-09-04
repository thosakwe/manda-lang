#include "token.hpp"

bool manda::analysis::tokenIsLonger(const Token &a, const Token &b)
{
  return a.text.length() < b.text.length();
}
