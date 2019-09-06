#include "vm_options.hpp"
#include <sstream>

using namespace manda::runtime;
using namespace std;

bool VMOptions::isREPL() const { return inputFile.empty(); }

ostream &manda::runtime::operator<<(std::ostream &out, const VMOptions &a) {
  out << "Manda VM options: " << endl;
  out << "Show help? " << a.showHelp << endl;
  out << "Developer mode? " << a.developerMode << endl;
  out << "REPL? " << a.isREPL() << endl;
  if (a.inputFile.empty()) {
    out << "Input file: " << a.inputFile << endl;
  } else {
    out << "<No input file was given>" << endl;
  }
  out << "Rest: " << a.rest.size() << endl;
  for (auto &arg : a.rest) {
    out << "  * " << arg << endl;
  }
  out << "Defines: " << a.defines.size() << endl;
  for (auto &p : a.defines) {
    out << "  * " << p.first << "=" << p.second << endl;
  }
  out << "Errors: " << a.parseErrors.size() << endl;
  for (auto &err : a.parseErrors) {
    out << "  * " << err << endl;
  }
  return out;
}

void VMOptions::parse(int argc, const char **argv) {
  vector<string> args;
  for (int i = 1; i < argc; i++) {
    args.emplace_back(argv[i]);
  }
  parse(args);
}

void VMOptions::parse(const vector<string> &args) {
  auto it = args.begin();
  while (it < args.end()) {
    auto s = *(it++);
    if (s.empty())
      continue;
    else if (encounteredBareDoubleDash) {
      rest.push_back(s);
    } else if (s == "--") {
      encounteredBareDoubleDash = true;
    } else if (s == "-h" || s == "--help") {
      showHelp = true;
    } else if (s == "--devel") {
      developerMode = true;
    } else if (s == "--define") {
      if (it < args.end()) {
        auto arg = *(it++);
        if (arg.empty()) {
          parseErrors.emplace_back(
              "--define cannot be followed by an empty string.");
        } else {
          parseDefine(arg, 0);
        }
      } else {
        parseErrors.emplace_back("--define cannot be the last argument.");
      }
    } else if (s.compare(0, 2, "-D")) {
      parseDefine(s, 2);
    } else {
      if (inputFile.empty()) {
        inputFile = s;
        encounteredBareDoubleDash =
            true; // TODO: Should all trailing arguments count as "last"?
      } else {
        rest.push_back(s);
      }
      //      if (s[0] != '-') {
      //        if (inputFile.empty()) {
      //          inputFile = s;
      //        } else {
      //          rest.push_back(s);
      //        }
      //      } else {
      //        ostringstream oss;
      //        oss << "Unrecognized option \"" << s << "\".";
      //        parseErrors.push_back(oss.str());
      //      }
    }
  }
}
void VMOptions::parseDefine(const string &str, size_t startIndex) {
  if (startIndex >= str.length()) {
    parseErrors.emplace_back("Missing name in -D or --define option.");
    return;
  }

  istringstream iss(str.substr(startIndex));
  string name, value;
  getline(iss, name, '=');
  if (name.empty()) {
    parseErrors.emplace_back("Missing name in -D or --define option.");
  } else if (iss.good()) {
    getline(iss, value);
  }
  defines[name] = value;
}
