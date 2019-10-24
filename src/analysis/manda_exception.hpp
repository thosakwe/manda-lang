#ifndef MANDA_MANDA_EXCEPTION_HPP
#define MANDA_MANDA_EXCEPTION_HPP
#include "location.hpp"
#include <exception>
#include <ostream>
#include <string>
#include <vector>

namespace manda::analysis {
class MandaException : public std::exception {
public:
  enum MandaExceptionType { ERROR, WARNING, HINT, INFO };
  MandaException(MandaExceptionType type, Location location,
                 std::string message);
  [[nodiscard]] MandaExceptionType getType() const;
  [[nodiscard]] const Location &getLocation() const;
  [[nodiscard]] const std::string &getMessage() const;
  friend std::ostream &operator<<(std::ostream &os, const MandaException &a);

private:
  Location location;
  MandaExceptionType type;
  std::string message;
};

class MandaErrorEmitter {
public:
  MandaErrorEmitter() = default;
  [[nodiscard]] bool hasErrors() const;
  [[nodiscard]] const std::vector<MandaException> &getErrors() const;

  void emit(MandaException::MandaExceptionType type, const Location &location,
            const std::string &message);
  void emitError(const Location &location, const std::string &message);
  void emitWarning(const Location &location, const std::string &message);
  void emitHint(const Location &location, const std::string &message);
  void emitInfo(const Location &location, const std::string &message);
  void pipe(MandaErrorEmitter &other);

private:
  MandaErrorEmitter *pipe_ = nullptr;
  std::vector<MandaException> errors;
};
} // namespace manda::analysis

#endif
