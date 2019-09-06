#ifndef MANDA_MODULE_HPP
#define MANDA_MODULE_HPP
#include "../analysis/expr_ctx.hpp"
#include "symbol_table.hpp"
#include <memory>
#include <string>
#include <vector>
//#include <unordered_map>

namespace manda::runtime {
class Module {
public:
  explicit Module(std::string name);
  //  const std::string &getName() const;
  //  void setName(std::string value);
  std::shared_ptr<SymbolTable> getSymbolTable();
  std::vector<std::unique_ptr<manda::analysis::ExprCtx>> &
  getTopLevelExpressions();
  //  std::shared_ptr<const SymbolTable> getSymbolTable() const;
  //  void addChild(std::shared_ptr<Module> &child);
  //  const std::shared_ptr<Module> findChild(std::string &name);

private:
  std::string name;
  std::shared_ptr<SymbolTable> symbolTable;
  std::vector<std::unique_ptr<manda::analysis::ExprCtx>> topLevelExpressions;
  //  std::unordered_map<std::string, std::shared_ptr<Module>> children;
};
} // namespace manda::runtime

#endif
