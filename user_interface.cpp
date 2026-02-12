#include "user_interface.hpp"
#include "inode.hpp"
#include <ios>
#include <iostream>
#include <stdexcept>

// Public

void UserInterface::process(const int argc, char *argv[]) {
  _readInput(argc, argv);
  _linker.setFlags(_flags);

  _file.open(_fileName, std::ios_base::in);
  std::string file_health = _checkFileHealth(_file);
  if (file_health.length() != 0) {
    std::cerr << "Error opening file: " << std::endl
              << file_health << std::endl;
  }

  INode *list = _parser.parse(&_file, _header, _tail);

  std::string linked_file = _linker.link(list, _header, _tail);

  std::cout << linked_file;

  _file.close();
}

// Private

// Obj.: Reads the input given by the user on the code call and puts it in an
//       understandable format for parsing.
void UserInterface::_readInput(const int argc, char *argv[]) {
  const std::string EXPECTED_ERR_MSG = ", expected:\n"
                                       "linkat [-r] file.ext header tail";
  const std::string NUM_ARGS_ERR_MSG =
      "Invalid number of arguments" + EXPECTED_ERR_MSG;
  const std::string ARGS_ERR_MSG = "Invalid arguments" + EXPECTED_ERR_MSG;
  const std::string FLAGS_ERR_MSG = "Invalid flags" + EXPECTED_ERR_MSG;

  if (argc < MIN_ARGS || argc > MAX_ARGS)
    throw std::invalid_argument(NUM_ARGS_ERR_MSG);

  if (argc == MIN_ARGS) {
    _fileName = argv[1];
    _header = argv[2];
    _tail = argv[3];
  } else {
    std::string flags = argv[1];
    if (flags[0] != '-')
      throw std::invalid_argument(ARGS_ERR_MSG);

    unsigned const int numFlags = flags.length() - 1;
    if (numFlags < NUM_FLAGS)
      throw std::invalid_argument(FLAGS_ERR_MSG);

    for (int i = 1; i < numFlags + 1; i++) {
      switch (flags[i]) {
      case 'r':
        _flags.recursive(true);
        continue;
      default:
        throw std::invalid_argument(FLAGS_ERR_MSG);
      }
    }

    _fileName = std::string(argv[2]);
    _header = std::string(argv[3]);
    _tail = std::string(argv[4]);
  }
}

// Obj.: Checks the integrity of a given file, returning an empty string if no
//       problem was found and returning a message stating the problem
//       otherwise.
std::string UserInterface::_checkFileHealth(std::fstream &file) {
  std::string msg = "";

  if (!file.is_open())
    msg = "File couldn't open";

  else if (file.bad())
    msg = "Bad file";

  else if (file.eof())
    msg = "File empty";

  else if (file.fail())
    msg = "Couldn't open file";

  return msg;
}
