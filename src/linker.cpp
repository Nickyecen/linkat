#include "linker.hpp"
#include "inode.hpp"
#include "parser.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>

const std::string Linker::link(INode *list, const std::string &header,
                               const std::string &tail) {
  std::stringstream result;

  // Itera sobre os INodes
  for (INode *it = list; it != nullptr; it = it->next()) {
    switch (it->type()) {
    // Blocos de texto são colocados na saída
    case INode::DataType::BLOCK:
      result << it->data();
      break;

    // Links são acessados
    case INode::DataType::LINK: {
      if (!it->dependency())
        throw std::invalid_argument("Linked file is invalid.");

      result << this->link(it->dependency(), header, tail);

      break;
    }
    }
  }

  return result.str();
}
