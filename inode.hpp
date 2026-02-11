#pragma once

#include <string>

class INode {
public:
  enum DataType { BLOCK, LINK };

private:
  const std::string _data;
  const DataType _type;

  INode *_prev;
  INode *_next;

public:
  INode(const std::string data, const DataType type, INode *prev = nullptr,
        INode *next = nullptr)
      : _data(data), _type(type), _prev(prev), _next(next) {}
  ~INode();

  INode *append(INode *next);

  const std::string data() { return _data; }
  const DataType type() { return _type; }

  INode *prev() { return _prev; }
  INode *next() { return _next; }

  void prev(INode *prev) { _prev = prev; }
  void next(INode *next) { _next = next; }
};
