#pragma once

#include <cstdint>

struct Flags {
public:
  enum Base {
    RECURSIVE = (uint8_t)(1 << 0),
  };

private:
  uint8_t _flags;

public:
  Flags(uint8_t flags = 0) : _flags(flags) {}

  bool recursive() { return _flags & RECURSIVE; }
  void recursive(bool val) {
    _flags = val ? _flags | RECURSIVE : _flags & ~RECURSIVE;
  }
};
