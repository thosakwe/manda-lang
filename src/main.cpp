#include <readline/history.h>
#include <readline/readline.h>
#include <string>

int main(int argc, char **argv) {
  // TODO: Run from file...
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (true) {
    std::string line(readline("manda> "));
    add_history(line.c_str());
  }
#pragma clang diagnostic pop
}