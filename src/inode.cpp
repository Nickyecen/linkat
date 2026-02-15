#include "inode.hpp"

INode::~INode() {
  if (_prev && _next) {
    _prev->next(_next);
    _next->prev(_prev);
  } else if (_prev && !_next) {
    _prev->next(nullptr);
  } else if (!_prev && _next) {
    _next->prev(nullptr);
  }
}

INode *INode::append(INode *next) {
  this->next(next);
  next->prev(this);

  return next;
}
