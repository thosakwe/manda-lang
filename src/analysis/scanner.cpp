#include "scanner.hpp"
#include <iostream>
#include <manda_flex_scanner.hpp>
#include <utility>

using namespace manda::analysis;
using namespace std;

Scanner::Scanner(string filename, string contents)
    : filename(std::move(filename)), contents(std::move(contents)) {
  buf = flex = nullptr;
  mandalex_init(&flex);
  buf = manda_scan_string(this->contents.c_str(), flex);
  begin = end = {filename, 1, 1};
}

Scanner::~Scanner() {
  if (buf != nullptr) {
    manda_delete_buffer((YY_BUFFER_STATE)buf, flex);
    buf = nullptr;
  }
  if (flex != nullptr) {
    mandalex_destroy(flex);
    flex = nullptr;
  }
}

Token Scanner::nextToken() {
  auto type = mandalex(flex);
  Token result = {type, {begin, end}, mandaget_text(flex)};
  end.offset += mandaget_leng(flex);
  end.column += mandaget_leng(flex);
  step();
}

void Scanner::columns(unsigned long count) {
  end.offset += mandaget_leng(flex);
  end.column += mandaget_leng(flex);
}

void Scanner::lines(unsigned long count) {
  end.offset += mandaget_leng(flex);
  end.line += mandaget_leng(flex);
  end.column = 1;
}

void Scanner::step() { begin = end; }

const string &Scanner::getFilename() const { return filename; }
