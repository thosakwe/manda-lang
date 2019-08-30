#include "type_reference.hpp"

w_lang::TypeReference::TypeReference(const std::string name) : name(name) {}

const std::string& w_lang::TypeReference::getName() const { return name; }
