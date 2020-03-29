#ifndef MANDA_MODULE_HPP
#define MANDA_MODULE_HPP
#include "expr_ctx.hpp"
#include "scope.hpp"
#include <memory>
#include <string>
#include <vector>
//#include <unordered_map>

namespace manda::analysis {
class Module {
public:
  explicit Module(std::string name);
  //  const std::string &getName() const;
  //  void setName(std::string value);
  std::shared_ptr<Scope> getSymbolTable();
  [[nodiscard]] const std::string &getName() const;
  std::vector<std::shared_ptr<manda::analysis::ExprCtx>> &
  getTopLevelExpressions();
  //  std::shared_ptr<const RuntimeScope> getSymbolTable() const;
  //  void addChild(std::shared_ptr<Module> &child);
  //  const std::shared_ptr<Module> findChild(std::string &name);

private:
  std::string name;
  std::shared_ptr<Scope> symbolTable;
  std::vector<std::shared_ptr<manda::analysis::ExprCtx>> topLevelExpressions;
  //  std::unordered_map<std::string, std::shared_ptr<Module>> children;
};
} // namespace manda::analysis

#endif
