#ifndef MANDA_VM_OPTIONS_HPP
#define MANDA_VM_OPTIONS_HPP
#include <string>
#include <unordered_map>
#include <vector>

namespace manda::runtime {
struct VMOptions {
  std::unordered_map<std::string, std::string> defines;
  std::vector<std::string> parseErrors;
  std::string inputFile;
  std::vector<std::string> rest;
  bool developerMode;
  bool showHelp;
  bool showVersion;
  [[nodiscard]] bool isREPL() const;
  void parse(int argc, const char **argv);
  void parse(const std::vector<std::string> &args);
  friend std::ostream &operator<<(std::ostream &out, const VMOptions &a);

private:
  bool encounteredBareDoubleDash; // i.e. manda ./file.mn -- a b c
  void parseDefine(const std::string &str, size_t startIndex);
};
} // namespace manda::runtime

#endif
