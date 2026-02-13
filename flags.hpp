#pragma once

#include <cstdint>

// Obj.: Estrutura que contém flags de execução que definem o comportamento do
//       programa
struct Flags {
public:
  enum Base {
    RECURSIVE = (uint8_t)(1 << 0), // Se ativa, links são feitos dentro dos
                                   // arquivos linkados ao arquivo principal
  };

private:
  uint8_t _flags; // inteiro contendo as flags, cada bit representa uma flag

public:
  Flags(uint8_t flags = 0) : _flags(flags) {}

  // Setters e Getters
  bool recursive() { return _flags & RECURSIVE; }
  void recursive(bool val) {
    _flags = val ? _flags | RECURSIVE : _flags & ~RECURSIVE;
  }
};
