#pragma once

#include <string>

// Obj.: Classe que contém as partes do arquivo análisado, criando uma lista de
//       Blocos de texto e Links de arquivos.
class INode {
public:
  enum DataType { BLOCK, LINK };

private:
  const std::string _data; // Texto se Block, nome do arquivo se Link
  const DataType _type;

  // Ponteiros de lista linkada
  INode *_prev;
  INode *_next;

public:
  INode(const std::string data, const DataType type, INode *prev = nullptr,
        INode *next = nullptr)
      : _data(data), _type(type), _prev(prev), _next(next) {}
  ~INode();

  // Obj.: Adiciona um INode como próximo da lista. Similar a void next(INode
  //       *next), mas automaticamente ajusta next e prev dos nodos.
  INode *append(INode *next);

  // Getters e Setters
  const std::string data() const { return _data; }
  const DataType type() const { return _type; }

  INode *prev() { return _prev; }
  INode *next() { return _next; }

  void prev(INode *prev) { _prev = prev; }
  void next(INode *next) { _next = next; }
};
