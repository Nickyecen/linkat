#pragma once

#include "inode.hpp"
#include <fstream>
#include <string>

class Parser {
private:
  enum State { READING_BLOCK, READING_LINK };

public:
  INode *parse(std::fstream *file, const std::string &header,
               const std::string &tail);

private:
  bool _checkLimiter(std::fstream *file, const std::string limiter);
};
