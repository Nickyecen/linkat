#pragma once

#include "flags.hpp"
#include "linker.hpp"
#include "parser.hpp"
#include <fstream>
#include <string>

class UserInterface {
public:
  // Constantes
  unsigned const int MIN_ARGS = 4;
  unsigned const int MAX_ARGS = 5;
  unsigned const int NUM_FLAGS = 1;

private:
  // Dados para parsing
  std::string _fileName = "";
  std::fstream _file;
  std::string _header = "";
  std::string _tail = "";

  // Flags de execução
  Flags _flags;

  // Etapas da execução
  Parser _parser;
  Linker _linker;

public:
  // Lê input e realiza processo de linkagem
  void process(const int argc, char *argv[]);

  // Getters e Setters
  void setFilename(const std::string fileName) { _fileName = fileName; }
  void setHeader(const std::string header) { _header = header; }
  void setTail(const std::string tail) { _tail = tail; }

private:
  // Métodos auxiliares
  void _readInput(const int argc, char *argv[]);
  std::string _checkFileHealth(std::fstream &file);
};
