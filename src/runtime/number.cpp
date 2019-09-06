#include "number.hpp"

manda::runtime::Number::Number(double value) : value(value) {}

double manda::runtime::Number::getValue() const { return value; }
