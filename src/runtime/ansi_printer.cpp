#include "ansi_printer.hpp"
#include <iostream>
#include <sstream>

using namespace std;

string manda::runtime::wrap(const string &s, const vector<int> &ansiCodes) {
  ostringstream oss;
  if (!(ansiCodes.empty())) {
    unsigned long i = 0;
    oss << "\x1b[";
    for (auto n : ansiCodes) {
      if (i++ > 0) {
        oss << ";";
      }
      oss << n;
    }
    oss << "m";
  }
  oss << s << "\x1b[0m";
  return oss.str();
}
string manda::runtime::bold(const string &s) { return wrap(s, {1}); }

string manda::runtime::cyan(const string &s) { return wrap(s, {36}); }

string manda::runtime::gray(const string &s) { return wrap(s, {30}); }

string manda::runtime::green(const string &s) { return wrap(s, {32}); }

string manda::runtime::red(const string &s) { return wrap(s, {31}); }

string manda::runtime::yellow(const string &s) { return wrap(s, {33}); }

string manda::runtime::white(const string &s) { return wrap(s, {37}); }

string manda::runtime::magenta(const string &s) { return wrap(s, {35}); }
