#include "type_reference.hpp"

manda::TypeReference::TypeReference(const std::string name) : name(name) {}

const std::string& manda::TypeReference::getName() const { return name; }
