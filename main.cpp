#include "link.hpp"
#include "read_input.hpp"
#include <cstdlib>
#include <string>

int main(int argc, char *argv[]) {
  std::string fileName;
  std::string header;
  std::string tail;
  bool recursive;

  readInput(argc, argv, fileName, header, tail, recursive);
  link(fileName, header, tail);

  return 0;
}

void link(const std::string fileName, const std::string header,
          const std::string tail);
