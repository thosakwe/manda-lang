#ifndef MANDA_MODULE_HPP
#define MANDA_MODULE_HPP
#include <memory>
#include <string>
//#include <unordered_map>

namespace manda {
namespace runtime {
class Module {
public:
  Module(std::string &name);
  const std::string &getName() const;
  //  void addChild(std::shared_ptr<Module> &child);
  //  const std::shared_ptr<Module> findChild(std::string &name);

private:
  std::string name;
  //  std::unordered_map<std::string, std::shared_ptr<Module>> children;
};
} // namespace runtime
} // namespace manda

#endif
