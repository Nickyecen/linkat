#pragma once

#include <map>
#include <string>
#include <vector>

class DependencyGraph {
private:
  std::map<const std::string, std::vector<std::string>> _graph;

public:
  DependencyGraph() { _graph = {}; }

  void addFile(const std::string &file);
  void addDependency(const std::string &file, const std::string &dependency);
  bool hasDependency(const std::string &file, const std::string &dependency);

  std::string toString();
};
