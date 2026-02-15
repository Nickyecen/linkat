#pragma once

#include "flags.hpp"
#include "inode.hpp"
#include <fstream>
#include <string>

class Parser {
private:
  Flags _flags;
  // Estados da máquina de estados de parsing
  enum class State { READING_BLOCK, READING_LINK };

public:
  Parser(const Flags flags = Flags()) : _flags(flags) {}

  // Obj.: Faz o parsing dos arquivos de texto para lista de INodes
  INode *parse(std::fstream *file, const std::string &header,
               const std::string &tail);

  // Obj.: Faz a leitura do arquivo de texto para um INode Bloco
  INode *read(std::fstream *file);

  // Getters e Setters
  void setFlags(Flags flags) { _flags = flags; }

private:
  // Obj.: Função auxiliar que chega se próximos caracteres do arquivo são o
  //       limitador passado
  bool _checkLimiter(std::fstream *file, const std::string limiter);
};
