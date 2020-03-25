#include "tuple_type.hpp"
#include <exception>
#include <sstream>

using namespace manda::analysis;
using namespace std;

TupleType::TupleType(std::vector<std::shared_ptr<Type>> itemTypes)
    : itemTypes(move(itemTypes)) {}

string TupleType::getName() const {
  ostringstream oss;
  oss << "(";
  for (unsigned long i = 0; i < itemTypes.size(); i++) {
    if (i > 0) {
      oss << ", ";
    }
    oss << itemTypes[i]->getName();
  }
  oss << ")";
  return oss.str();
}