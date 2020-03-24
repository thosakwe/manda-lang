#include "string_type.hpp"
#include "ansi_printer.hpp"
#include "interpreter.hpp"
#include <iostream>
#include <sstream>

using namespace manda::runtime;
using namespace std;

string StringType::getName() const { return "String"; }