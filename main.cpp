#include "link.hpp"
#include "read_input.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string fileName;
  std::string header;
  std::string tail;
  bool recursive;

  readInput(argc, argv, fileName, header, tail, recursive);
  std::cout << link(fileName, header, tail, recursive);

  return 0;
}
