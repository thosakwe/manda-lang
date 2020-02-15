#include "ir_instruction.hpp"
#include <sstream>

using namespace manda::ir;
using namespace std;

string hexstr(uint64_t n) {
  ostringstream oss;
  oss << "0x";
  oss << hex << n;
  return oss.str();
}

ostream &manda::ir::operator<<(ostream &out, const IRInstruction &a) {
  switch (a.opcode) {
  case IRInstruction::FLOAT_CONST_64:
    out << "FLOAT_CONST_64 " << a.operandf64;
    break;
  case IRInstruction::INT_CONST_64:
    out << "INT_CONST_64 " << hexstr(a.operand64);
    break;
  case IRInstruction::MUL:
    out << "MUL";
    break;
  case IRInstruction::DIV:
    out << "DIV";
    break;
  case IRInstruction::ADD:
    out << "ADD";
    break;
  case IRInstruction::SUB:
    out << "SUB";
    break;
  case IRInstruction::JUMP:
    out << "JUMP " << hexstr(a.operand64);
    break;
  case IRInstruction::JUMP_IFEQ:
    out << "JUMP_IFEQ " << hexstr(a.operand64);
    break;
  case IRInstruction::JUMP_IFNEQ:
    out << "JUMP_IFNEQ " << hexstr(a.operand64);
    break;
  case IRInstruction::JUMP_IFLT:
    out << "JUMP_IFLT " << hexstr(a.operand64);
    break;
  case IRInstruction::JUMP_IFLTE:
    out << "JUMP_IFLTE " << hexstr(a.operand64);
    break;
  case IRInstruction::JUMP_IFGT:
    out << "JUMP_IFGT " << hexstr(a.operand64);
    break;
  case IRInstruction::JUMP_IFGTE:
    out << "JUMP_IFGTE " << hexstr(a.operand64);
    break;
  case IRInstruction::CALL:
    out << "CALL " << hexstr(a.operand64);
    break;
  case IRInstruction::RETURN:
    out << "RETURN";
    break;
  case IRInstruction::GET_MEMORY:
    out << "GET_MEMORY " << hexstr(a.operand64);
    break;
  case IRInstruction::SET_MEMORY:
    out << "SET_MEMORY " << hexstr(a.operand64);
    break;
  case IRInstruction::GET_PARAM:
    out << "GET_PARAM " << hexstr(a.operand64);
    break;
  case IRInstruction::GET_LOCAL:
    out << "GET_LOCAL " << hexstr(a.operand64);
    break;
  case IRInstruction::SET_LOCAL:
    out << "SET_LOCAL " << hexstr(a.operand64);
    break;
  case IRInstruction::ALLOC:
    out << "ALLOC " << hexstr(a.operand64);
    break;
  case IRInstruction::MARK:
    out << "MARK";
    break;
  }
  return out;
}
