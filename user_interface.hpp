#pragma once

#include "flags.hpp"
#include "linker.hpp"
#include "parser.hpp"
#include <fstream>
#include <string>

class UserInterface {
public:
  unsigned const int MIN_ARGS = 4;
  unsigned const int MAX_ARGS = 5;
  unsigned const int NUM_FLAGS = 1;

private:
  std::string _fileName = "";
  std::fstream _file;
  std::string _header = "";
  std::string _tail = "";

  Flags _flags;

  Parser _parser;
  Linker _linker;

public:
  void process(const int argc, char *argv[]);

  void setFilename(const std::string fileName) { _fileName = fileName; }
  void setHeader(const std::string header) { _header = header; }
  void setTail(const std::string tail) { _tail = tail; }

private:
  void _readInput(const int argc, char *argv[]);
  std::string _checkFileHealth(std::fstream &file);
};
