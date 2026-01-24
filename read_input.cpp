#include "read_input.hpp"
#include <iostream>
#include <string>

void errorInvalidFormat();
void errorInvalidFlags();

/*  read_input: le os inputs dos usuarios e extrai nome do arquivo, header e
 * tail
 *  - argv[0] e o nome do executavel
 *  - argv[1] e o nome do arquivo | flag
 *  - argv[2] e o header | nome do arquivo
 *  - argv[3] e o tail | header
 *  - argv[4] e o tail
 */
void readInput(const int argc, char *const argv[], std::string &fileName,
               std::string &header, std::string &tail, bool &recursive) {
  if (argc < 4 || argc > 5)
    errorInvalidFormat();

  if (argc == 4) {
    recursive = false;
    fileName = std::string(argv[1]);
    header = std::string(argv[2]);
    tail = std::string(argv[3]);
  } else {
    std::string flags = std::string(argv[1]);
    if (flags[0] != '-')
      errorInvalidFormat();

    unsigned const int numFlags = flags.length();
    if (numFlags < 2)
      errorInvalidFlags();

    for (int i = 1; i < numFlags; i++) {
      switch (flags[i]) {
      case 'r':
        recursive = true;
        continue;
      default:
        errorInvalidFlags();
      }
    }

    fileName = std::string(argv[2]);
    header = std::string(argv[3]);
    tail = std::string(argv[4]);
  }
}

// Erros
void errorInvalidFormat() {
  std::cerr << "Invalid call. Format:" << std::endl
            << "linkat [-r] filename.ext \"expected_header\" \"expected_tail\""
            << std::endl;
  exit(EXIT_FAILURE);
}

void errorInvalidFlags() {
  std::cerr << "Invalid flags called. Valid flags are:" << std::endl
            << "-r" << std::endl;
  exit(EXIT_FAILURE);
}
