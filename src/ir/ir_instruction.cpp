#include "ir_instruction.hpp"

using namespace manda::ir;
using namespace std;

ostream &manda::ir::operator<<(ostream &out, const IRInstruction &a) {
  // TODO: Print operands
  switch (a.opcode) {
  case IRInstruction::FLOAT_CONST_64:
    out << "FLOAT_CONST_64";
    break;
  case IRInstruction::INT_CONST_64:
    out << "INT_CONST_64";
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
    out << "JUMP";
    break;
  case IRInstruction::JUMP_IFEQ:
    out << "JUMP_IFEQ";
    break;
  case IRInstruction::JUMP_IFNEQ:
    out << "JUMP_IFNEQ";
    break;
  case IRInstruction::JUMP_IFLT:
    out << "JUMP_IFLT";
    break;
  case IRInstruction::JUMP_IFLTE:
    out << "JUMP_IFLTE";
    break;
  case IRInstruction::JUMP_IFGT:
    out << "JUMP_IFGT";
    break;
  case IRInstruction::JUMP_IFGTE:
    out << "JUMP_IFGTE";
    break;
  case IRInstruction::CALL:
    out << "CALL";
    break;
  case IRInstruction::RETURN:
    out << "RETURN";
    break;
  case IRInstruction::GET_MEMORY:
    out << "GET_MEMORY";
    break;
  case IRInstruction::SET_MEMORY:
    out << "SET_MEMORY";
    break;
  case IRInstruction::GET_PARAM:
    out << "GET_PARAM";
    break;
  case IRInstruction::GET_LOCAL:
    out << "GET_LOCAL";
    break;
  case IRInstruction::SET_LOCAL:
    out << "SET_LOCAL";
    break;
  case IRInstruction::ALLOC:
    out << "ALLOC";
    break;
  case IRInstruction::MARK:
    out << "MARK";
    break;
  }
  return out;
}
