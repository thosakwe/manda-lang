#ifndef MANDA_ANSI_PRINTER_HPP
#define MANDA_ANSI_PRINTER_HPP
#include <ostream>
#include <string>
#include <vector>

namespace manda::runtime {
std::string wrap(const std::string &s, const std::vector<int> &ansiCodes);
std::string bold(const std::string &s);
std::string cyan(const std::string &s);
std::string gray(const std::string &s);
std::string green(const std::string &s);
std::string red(const std::string &s);
std::string yellow(const std::string &s);
std::string white(const std::string &s);
std::string magenta(const std::string &s);
} // namespace manda::runtime

#endif
