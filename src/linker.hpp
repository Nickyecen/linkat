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

  // Obj.: Faz a linkagem dos arquivos a partir de uma lista de INodes. Os
  //       parâmetros de header e tail devem ser passados caso fazer-se-á uma
  //       linkagem recursiva, são opcionais caso contrário.
  const std::string link(INode *list, const std::string &header = "",
                         const std::string &tail = "");

  // Getters e Setters
  void setFlags(Flags flags) { _flags = flags; }
};
