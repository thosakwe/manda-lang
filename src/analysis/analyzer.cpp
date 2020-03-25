#include "analyzer.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

Analyzer::Analyzer(VMOptions options) : vmOptions(move(options)) {}
