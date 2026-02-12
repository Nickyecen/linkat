#pragma once

#include "inode.hpp"
#include <fstream>
#include <string>

class Parser {
private:
  // Estados da máquina de estados de parsing
  enum State { READING_BLOCK, READING_LINK };

public:
  // Obj.: Faz o parsing dos arquivos de texto para lista de INodes
  INode *parse(std::fstream *file, const std::string &header,
               const std::string &tail);

private:
  // Obj.: Função auxiliar que chega se próximos caracteres do arquivo são o
  //       limitador passado
  bool _checkLimiter(std::fstream *file, const std::string limiter);
};
