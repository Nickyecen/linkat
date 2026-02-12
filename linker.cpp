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

  for (INode *it = list; it != nullptr; it = it->next()) {
    switch (it->type()) {
    case INode::DataType::BLOCK:
      result << it->data();
      break;

    case INode::DataType::LINK: {
      std::fstream linked_file(it->data(), std::ios_base::in);

      if (linked_file.fail())
        throw std::invalid_argument("Linked file is invalid.");

      if (_flags.recursive()) {
        INode *link_list = _parser.parse(&linked_file, header, tail);
        result << this->link(link_list, header, tail);
      } else {
        result << linked_file.rdbuf();
      }

      linked_file.close();
      break;
    }
    }
  }

  return result.str();
}
