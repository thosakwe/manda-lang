#include "type_resolver.hpp"
#include <sstream>
#include <utility>

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

TypeResolver::TypeResolver(Interpreter &interpreter,
                           shared_ptr<SymbolTable> scope)
    : interpreter(interpreter), scope(std::move(scope)) {}

const shared_ptr<Type> &TypeResolver::getLastType() const { return lastType; }

void TypeResolver::visitTypeRef(TypeRefCtx &ctx) {
  auto symbol = scope->resolve(ctx.name);
  if (holds_alternative<monostate>(symbol)) {
    ostringstream oss;
    oss << "The name '";
    oss << ctx.name << "' does not exist in this context.";
    interpreter.reportError(ctx.location, oss.str());
    lastType = nullptr;
  } else if (holds_alternative<shared_ptr<Object>>(symbol)) {
    ostringstream oss;
    oss << "The value of symbol '";
    oss << ctx.name << "' is a value, not a type.";
    interpreter.reportError(ctx.location, oss.str());
    lastType = nullptr;
  } else if (holds_alternative<shared_ptr<Type>>(symbol)) {
    lastType = get<shared_ptr<Type>>(symbol);
  } else {
    // TODO: This will probably never be called; but should it throw a error?
    lastType = nullptr;
  }
}
