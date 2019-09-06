#include "number.hpp"
#include <iostream>

manda::runtime::Number::Number(double value) : value(value) {}

double manda::runtime::Number::getValue() const { return value; }

void manda::runtime::Number::print(std::ostream &out) const { out << value; }
