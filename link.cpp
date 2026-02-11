#include "link.hpp"
#include "dependency_graph.hpp"
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define READING_BODY 0
#define READING_HEADER 1
#define READING_FILENAME 2
#define READING_TAIL 3

DependencyGraph dependencyGraph;

std::string readFile(const std::string &fileName);

// Errors
void errorInvalidFile(const std::string &fileName);
void errorInvalidHeader(const std::string &fileName);
void errorInvalidTail(const std::string &fileName);
void errorExecution(const std::string &fileName);
void errorMissingTail(const std::string &fileName,
                      const std::string &includedFile);
void errorIncompleteTail(const std::string &fileName);
void errorExecution(const std::string &fileName);

std::string link(const std::string &fileName, const std::string &header,
                 const std::string &tail, const bool recursive) {
  if (header.length() == 0)
    errorInvalidHeader(fileName);
  if (tail.length() == 0)
    errorInvalidTail(fileName);

  std::fstream file = std::fstream(fileName, std::ios_base::in);
  if (!file.is_open() || file.bad())
    errorInvalidFile(fileName);

  std::vector<char> buffer;
  std::stringstream includedFile;
  std::stringstream out;

  dependencyGraph.addFile(fileName);

  int state = READING_BODY;
  int afixPtr = 1;
  // Loop de leitura do arquivo de texto
  for (char curChar = file.get(); !file.eof(); curChar = file.get()) {
    switch (state) {
    // Lendo corpo do texto, fora de um header e tail
    case READING_BODY:
      // Se o caractere for o inicio do header, e preciso testar se nao e um
      // header
      if (curChar == header[0]) {
        buffer.push_back(curChar);
        state = READING_HEADER;
        break;
      }

      // Senao, imprime o caractere
      out << curChar;
      break;
    // Lendo header
    case READING_HEADER:
      // Se leu o header inteiro
      if (afixPtr == header.length()) {
        afixPtr = 1;
        buffer.clear();

        // Se o primeiro caractere apos o header for o inicio do tail, checa se
        // e tail
        if (curChar == tail[0]) {
          buffer.push_back(curChar);
          state = READING_TAIL;
          break;
        }

        // Senao, le caractere parte do nome do arquivo
        includedFile << curChar;
        state = READING_FILENAME;
        break;
      }

      // Senao, se caractere lido nao faz parte do header, volta para leitura do
      // corpo
      if (header[afixPtr++] != curChar) {
        out << buffer[0];
        buffer.push_back(curChar);
        bool subHeader;

        // Checa se header inclui proprio inicio (exemplo: se header e
        // "heheader" e ele esta lendo "heheheader" no texto)
        for (int i = 1; i < buffer.size(); i++) {
          subHeader = true;
          for (int j = i; j < buffer.size(); j++) {
            if (buffer[j] == header[j - i])
              continue;
            subHeader = false;
            break;
          }

          // Se encontrou um subheader
          if (subHeader) {
            afixPtr -= i;
            buffer.erase(buffer.begin(), buffer.begin() + i);
            break;
          }

          out << buffer[i];
        }

        // Se encontrou subheader, segue lendo header
        if (subHeader)
          break;

        // Senao, passa a ler corpo
        afixPtr = 1;

        buffer.clear();
        state = READING_BODY;
        break;
      }

      // Senao, continua checando header
      buffer.push_back(curChar);
      break;
    // Lendo nome de arquivo a ter seu texto adicionado
    case READING_FILENAME:
      // Se caractere lido for inicio do tail, checa se nao e tail
      if (curChar == tail[0]) {
        buffer.push_back(curChar);
        state = READING_TAIL;
        break;
      }

      // Senao, le caractere do nome do arquivo
      includedFile << curChar;
      break;
    // Lendo tail
    case READING_TAIL:
      // Se leu tail inteira
      if (afixPtr == tail.length()) {
        std::string includedFileStr = includedFile.str();

        dependencyGraph.addFile(includedFileStr);
        dependencyGraph.addDependency(fileName, includedFileStr);

        // Se for recursivo
        if (recursive) {
          // Faz link de includes no novo arquivo
          out << link(includedFileStr, header, tail, true);
        } else {
          // Senao, apenas imprime o arquivo
          out << readFile(includedFileStr);
        }

        afixPtr = 1;
        buffer.clear();
        includedFile.str("");

        // Se o caractere lido for inicio do header, checa se e header
        if (curChar == header[0]) {
          buffer.push_back(curChar);
          state = READING_HEADER;
          break;
        }

        // Senao, faz leitura do corpo
        out << curChar;
        state = READING_BODY;
        break;
      }

      // Senao, se caractere lido nao for parte do tail, volta a ler nome do
      // arquivo
      if (tail[afixPtr++] != curChar) {
        includedFile << buffer[0];
        buffer.push_back(curChar);
        bool subTail;

        // Checa se tail inclui proprio inicio (exemplo: se tail e
        // "tatail" e ele esta lendo "tatatail" no texto)
        for (int i = 1; i < buffer.size(); i++) {
          subTail = true;
          for (int j = i; j < buffer.size(); j++) {
            if (buffer[j] == tail[j - i])
              continue;
            subTail = false;
            break;
          }

          // Se encontrou um subtail
          if (subTail) {
            afixPtr -= i;
            buffer.erase(buffer.begin(), buffer.begin() + i);
            break;
          }

          includedFile << buffer[i];
        }

        // Se encontrou subtail, segue lendo tail
        if (subTail)
          break;

        afixPtr = 1;

        buffer.clear();
        state = READING_FILENAME;
        break;
      }

      // Senao, le caractere do tail
      buffer.push_back(curChar);
      break;

    // Caso estado assuma valor impossivel
    default:
      errorExecution(fileName);
    }
  }

  file.close();

  // Finalizacao dos includes
  switch (state) {
  // Se finalizou lendo corpo, sucesso
  case READING_BODY:
    break;
  // Se finalizou lendo header, o header era um corpo, imprime e sucesso
  case READING_HEADER:
    for (char c : buffer) {
      out << c;
    }
    break;
  // Se finalizou lendo nome do arquivo, estava sem tail
  case READING_FILENAME:
    errorMissingTail(fileName, includedFile.str());
  // Se finalizou lendo tail
  case READING_TAIL:
    // Se leu tail inteiro, sucesso
    if (afixPtr == tail.length())
      break;

    // Senao, faltava caracteres no tail
    errorMissingTail(fileName, includedFile.str());
  }

  std::cout << dependencyGraph.toString();

  return out.str();
}

std::string readFile(const std::string &fileName) {
  std::fstream file = std::fstream(fileName, std::ios_base::in);
  if (!file.is_open() || file.bad())
    errorInvalidFile(fileName);

  std::stringstream out;
  for (char curChar = file.get(); !file.eof(); curChar = file.get())
    out << curChar;

  file.close();

  return out.str();
}

// Errors
void errorInvalidFile(const std::string &fileName) {
  std::cerr << "Invalid file name:" << std::endl << fileName << std::endl;
  exit(EXIT_FAILURE);
}
void errorInvalidHeader(const std::string &fileName) {
  std::cerr << "Invalid header on " << fileName
            << ". Header must have at least one character." << std::endl;
  exit(EXIT_FAILURE);
}
void errorInvalidTail(const std::string &fileName) {
  std::cerr << "Invalid tail on " << fileName
            << ". Tail must have at least one character." << std::endl;
  exit(EXIT_FAILURE);
}
void errorExecution(const std::string &fileName) {
  std::cerr << "Undiagnosed execution error on " << fileName
            << ". Please report this error." << std::endl;
  exit(EXIT_FAILURE);
}
void errorMissingTail(const std::string &fileName,
                      const std::string &includedFile) {
  std::cerr << "Missing tail on file " << fileName
            << " for file name:" << std::endl
            << includedFile << std::endl;
  exit(EXIT_FAILURE);
}
void errorIncompleteTail(const std::string &fileName) {
  std::cerr << "Tail incomplete at the end of file " << fileName << "."
            << std::endl;
  exit(EXIT_FAILURE);
}
