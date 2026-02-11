#include "parser.hpp"
#include "inode.hpp"
#include <ios>
#include <sstream>
#include <stdexcept>

INode *Parser::parse() {
  if (!_file)
    throw std::invalid_argument("Parsing without a file");

  if (_header.length() == 0)
    throw std::invalid_argument("Parsing without a header");

  if (_tail.length() == 0)
    throw std::invalid_argument("Parsing without a tail");

  INode *init = new INode("", INode::DataType::BLOCK);
  INode *current = init;

  State state = State::READING_BLOCK;

  std::stringstream data;
  char symbol;
  while (!_file->eof()) {
    symbol = _file->get();

    switch (state) {
    case State::READING_BLOCK:
      if (symbol == _header[0] && _checkHeader()) {
        current =
            current->append(new INode(data.str(), INode::DataType::BLOCK));
        data.str("");
        state = State::READING_LINK;
        break;
      }
      data << symbol;
      break;
    case State::READING_LINK:
      if (symbol == _tail[0] && _checkTail()) {
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

bool Parser::_checkHeader() { return _checkLimiter(_header); }

bool Parser::_checkTail() { return _checkLimiter(_tail); }

bool Parser::_checkLimiter(const std::string limiter) {
  _file->seekg(-1, std::ios_base::cur);

  char symbol;
  int i = 0;
  while (!_file->eof() && i < limiter.length()) {
    symbol = _file->get();
    if (symbol != limiter[i]) {
      _file->seekg(-i, std::ios_base::cur);
      return false;
    }
    i++;
  }

  return i == limiter.length();
}
