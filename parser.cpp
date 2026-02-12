#include "parser.hpp"
#include "inode.hpp"
#include <sstream>
#include <stdexcept>
#include <string>

INode *Parser::parse(std::fstream *file, const std::string &header,
                     const std::string &tail) {
  if (!file)
    throw std::invalid_argument("Parsing without a file");

  if (header.length() == 0)
    throw std::invalid_argument("Parsing without a header");

  if (tail.length() == 0)
    throw std::invalid_argument("Parsing without a tail");

  INode *init = new INode("", INode::DataType::BLOCK);
  INode *current = init;

  State state = State::READING_BLOCK;

  std::stringstream data;
  char symbol;
  while (file->get(symbol)) {
    switch (state) {
    case State::READING_BLOCK:
      if (symbol == header[0] && _checkLimiter(file, header)) {
        current =
            current->append(new INode(data.str(), INode::DataType::BLOCK));
        data.str("");
        state = State::READING_LINK;
        break;
      }
      data << symbol;
      break;
    case State::READING_LINK:
      if (symbol == tail[0] && _checkLimiter(file, tail)) {
        current = current->append(new INode(data.str(), INode::DataType::LINK));
        data.str("");
        state = State::READING_BLOCK;
        break;
      }
      data << symbol;
      break;
    }
  }

  switch (state) {
  case State::READING_BLOCK: {
    std::string data_str = data.str();
    if (!data_str.empty()) {
      current->append(new INode(data_str, INode::DataType::BLOCK));
    }
    INode *buffer = init->next();
    delete init;
    return buffer;
  }
  case State::READING_LINK:
    throw std::invalid_argument("File has header but no tail");
    break;
  }

  return nullptr;
}

bool Parser::_checkLimiter(std::fstream *file, const std::string limiter) {
  file->seekg(-1, std::ios_base::cur);

  char symbol;
  int i = 0;
  while (!file->eof() && i < limiter.length()) {
    symbol = file->get();
    if (symbol != limiter[i]) {
      file->seekg(-i, std::ios_base::cur);
      return false;
    }
    i++;
  }

  return i == limiter.length();
}
