#include "link.hpp"
#include "read_input.hpp"
#include "user_interface.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  UserInterface ui;
  ui.process(argc, argv);

  return 0;
}
