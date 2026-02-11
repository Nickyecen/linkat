#include "dependency_graph.hpp"
#include <iterator>
#include <sstream>
#include <utility>
#include <vector>

void DependencyGraph::addFile(const std::string &file) {
  auto it = _graph.find(file);

  if (it == _graph.end()) {
    std::vector<std::string> dependencies;
    _graph.emplace(std::make_pair(file, dependencies));
  }
}

void DependencyGraph::addDependency(const std::string &file,
                                    const std::string &dependency) {
  _graph.at(file).push_back(dependency);
}

bool DependencyGraph::hasDependency(const std::string &file,
                                    const std::string &dependency) {
  auto vec = _graph.at(file);
  for (auto dep : vec) {
    if (dep == dependency)
      return true;
  }
  return false;
}

std::string DependencyGraph::toString() {
  std::stringstream ss;

  ss << "{" << std::endl;

  for (auto &pair : _graph) {
    ss << pair.first << ": [";
    for (auto &str : pair.second) {
      ss << str << ", ";
    }
    ss << "];" << std::endl;
  }

  ss << "}" << std::endl;

  return ss.str();
}
