#include "user_interface.hpp"

// Entrada esperada:
// linkat [-r] filename.ext header tail
int main(int argc, char *argv[]) {
  UserInterface ui;
  ui.process(argc, argv);

  return 0;
}
