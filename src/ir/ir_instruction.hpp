#ifndef MANDA_IR_INSTRUCTION_HPP
#define MANDA_IR_INSTRUCTION_HPP
#include <cstdint>
#include <ostream>

namespace manda::ir {
struct IRInstruction {
  enum Opcode {
    // Push a 64-bit float constant to the stack.
    FLOAT_CONST_64,
    // Push a 64-bit integer constant to the stack.
    INT_CONST_64,
    // Multiply two integers on the stack.
    MUL,
    // Divide two integers on the stack ($0 / $1).
    DIV,
    // Add two integers on the stack.
    ADD,
    // Subtract two integers on the stack ($0 - $1).
    SUB,
    // Jump to operand1.
    JUMP,
    // Jump, if flags == 0
    JUMP_IFEQ,
    // Jump, if flags != 0
    JUMP_IFNEQ,
    // Jump if LT bit is set
    JUMP_IFLT,
    // Jump if flags == 0, or LT bit is set
    JUMP_IFLTE,
    // Jump if GT bit is set
    JUMP_IFGT,
    // Jump if flags == 0, or GT bit is set
    JUMP_IFGTE,
    // Jump to operand1, with stack values interpreted as parameters.
    CALL,
    // Return to the caller.
    RETURN,
    // Reads operand1 bytes from address $0, and places them on the stack.
    GET_MEMORY,
    // Writes operand1 bytes from the stack, and stores them at address $0.
    SET_MEMORY,
    // Read the value of params[operand1] onto the stack.
    GET_PARAM,
    // Read the value of locals[operand1] onto the stack.
    GET_LOCAL,
    // Write a value from the stack into locals[operand1].
    SET_LOCAL,
    // Allocate, and mark, operand1 bytes of dynamic memory; places a 64-bit
    // pointer on the stack.
    ALLOC,
    // Mark a pointer as "in use" for this round of GC sweeping.
    MARK,
  };

  Opcode opcode;
  union {
    struct {
      uint16_t operand1, operand2, operand3, operand4;
    } operands16;
    struct {
      uint32_t operand1, operand2;
    } operands32;
    uint64_t operand64;
    double operandf64;
  };
};

std::ostream &operator<<(std::ostream &out, const IRInstruction &a);
} // namespace manda::ir

#endif
