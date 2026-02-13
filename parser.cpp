#include "parser.hpp"
#include "inode.hpp"
#include <sstream>
#include <stdexcept>
#include <string>

INode *Parser::parse(std::fstream *file, const std::string &header,
                     const std::string &tail) {
  // Checa entrada
  if (!file)
    throw std::invalid_argument("Parsing without a file");

  if (header.length() == 0)
    throw std::invalid_argument("Parsing without a header");

  if (tail.length() == 0)
    throw std::invalid_argument("Parsing without a tail");

  // INode auxiliar para simplificar lógica do loop
  INode *init = new INode("", INode::DataType::BLOCK);
  INode *current = init;

  State state = State::READING_BLOCK;

  std::stringstream data; // Dado do INode atual
  char symbol;            // Símbolo atual lido do arquivo
  while (file->get(symbol)) {
    switch (state) {
    case State::READING_BLOCK:
      // Se encontrou header
      if (symbol == header[0] && _checkLimiter(file, header)) {
        // Cria INode com dados atuais
        current =
            current->append(new INode(data.str(), INode::DataType::BLOCK));
        data.str("");
        // Começa a ler nome do arquivo de link
        state = State::READING_LINK;
        break;
      }
      // Senão, armazena símbolo no bloco
      data << symbol;
      break;
    case State::READING_LINK:
      // Se encontrou tail
      if (symbol == tail[0] && _checkLimiter(file, tail)) {
        // Cria INode com dados atuais
        current = current->append(new INode(data.str(), INode::DataType::LINK));
        data.str("");
        // Começa a ler novo bloco
        state = State::READING_BLOCK;
        break;
      }
      // Senão, armazena símbolo no link
      data << symbol;
      break;
    }
  }

  // Término da leitura do arquivo
  switch (state) {
  case State::READING_BLOCK: {
    std::string data_str = data.str();
    // Se sobrou textos não adicionados a um novo bloco, adiciona
    if (!data_str.empty()) {
      current->append(new INode(data_str, INode::DataType::BLOCK));
    }

    // Remove INode auxiliar do início
    INode *buffer = init->next();
    delete init;

    // Retorna lista de INodes
    return buffer;
  }
  // Se leitura terminou durante a leitura de um link, há a falta de um tail
  case State::READING_LINK:
    throw std::invalid_argument("File has header but no tail");
    break;
  }

  // Segmento inalcançável do código
  throw std::runtime_error(
      "Unreachable code segment reached after Parsing. Report this issue.");
  return nullptr;
}

bool Parser::_checkLimiter(std::fstream *file, const std::string limiter) {
  // Volta um caractere, pois foi lido para decidir se a chamada da função
  // ocorreria
  file->seekg(-1, std::ios_base::cur);

  char symbol;
  int i = 0;
  // Checa se delimitador é igual à sequência no arquivo
  while (file->get(symbol) && i < limiter.length()) {
    // Se não for, põe agulha de leitura de volta e retorna falso
    if (symbol != limiter[i]) {
      file->seekg(-i, std::ios_base::cur);
      return false;
    }
    i++;
  }

  // Se for, retorna true (comprimento é chacado para verificar se loop não
  // encerrou por EOF)
  return i == limiter.length();
}
