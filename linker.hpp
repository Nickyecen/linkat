#pragma once

#include "flags.hpp"
#include "inode.hpp"
#include "parser.hpp"

class Linker {
private:
  Parser _parser;
  Flags _flags;

public:
  Linker(const Flags flags = Flags()) : _flags(flags) {}

  const std::string link(INode *list, const std::string &header = "",
                         const std::string &tail = "");

  void setFlags(Flags flags) { _flags = flags; }
};
