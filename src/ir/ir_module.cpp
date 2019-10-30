#include "ir_module.hpp"

using namespace manda::ir;
using namespace std;

IRModule::IRModule(string name) : name(move(name)) {}

IRModule::IRModule(string name, shared_ptr<IRModule> parent)
    : name(move(name)), parent(move(parent)) {}

const shared_ptr<IRModule> &IRModule::getParent() const { return parent; }

const string &IRModule::getName() const { return name; }
