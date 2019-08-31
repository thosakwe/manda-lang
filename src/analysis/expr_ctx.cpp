#include "expr_ctx.hpp"

manda::analysis::BoolLiteralCtx::BoolLiteralCtx(bool value) : value(value) {}

manda::analysis::NumberLiteralCtx::NumberLiteralCtx(double value)
    : value(value) {}

manda::analysis::StringLiteralCtx::StringLiteralCtx(std::string &value)
    : value(value) {}
