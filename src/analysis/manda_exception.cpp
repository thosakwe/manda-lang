#include "manda_exception.hpp"
#include "../runtime/ansi_printer.hpp"

using namespace manda::analysis;
using namespace manda::runtime;
using namespace std;

MandaException::MandaException(MandaException::MandaExceptionType type,
                               Location location, string message)
    : type(type), location(move(location)), message(move(message)) {}

MandaException::MandaExceptionType MandaException::getType() const {
  return type;
}

const Location &MandaException::getLocation() const { return location; }

const string &MandaException::getMessage() const { return message; }

MandaErrorEmitter::MandaErrorEmitter(const MandaErrorEmitter &other) {
  errors.insert(errors.end(), other.errors.begin(), other.errors.end());
}

bool MandaErrorEmitter::hasErrors() const { return !errors.empty(); }

const vector<MandaException> &MandaErrorEmitter::getErrors() const {
  return errors;
}

void MandaErrorEmitter::emit(MandaException::MandaExceptionType type,
                             const Location &location, const string &message) {
  errors.emplace_back(type, location, message);
}

void MandaErrorEmitter::emitError(const Location &location,
                                  const string &message) {
  emit(MandaException::ERROR, location, message);
}

void MandaErrorEmitter::emitWarning(const Location &location,
                                    const string &message) {
  emit(MandaException::WARNING, location, message);
}

void MandaErrorEmitter::emitHint(const Location &location,
                                 const string &message) {
  emit(MandaException::HINT, location, message);
}

void MandaErrorEmitter::emitInfo(const Location &location,
                                 const string &message) {
  emit(MandaException::INFO, location, message);
}

std::ostream &manda::analysis::operator<<(std::ostream &os,
                                          const MandaException &a) {
  switch (a.getType()) {
  case MandaException::ERROR:
    os << "error";
    break;
  case MandaException::WARNING:
    os << "warning";
    break;
  case MandaException::HINT:
    os << "hint";
    break;
  case MandaException::INFO:
    os << "info";
    break;
  }

  os << ": " << a.getLocation() << ": " << a.getMessage();
  return os;
}
