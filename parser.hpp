#pragma once

#include "inode.hpp"
#include <fstream>
#include <string>

class Parser {
private:
  std::fstream *_file;
  std::string _header = "";
  std::string _tail = "";

  enum State { READING_BLOCK, READING_LINK };

public:
  INode *parse();

  void setFile(std::fstream &file) { _file = &file; }
  void setHeader(const std::string &header) { _header = header; }
  void setTail(const std::string &tail) { _tail = tail; }

private:
  bool _checkHeader();
  bool _checkTail();
  bool _checkLimiter(const std::string limiter);
};
