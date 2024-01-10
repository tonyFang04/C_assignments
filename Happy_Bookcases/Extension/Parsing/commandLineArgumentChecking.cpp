#include "misc.hpp"


CommandLineArgumentChecker::CommandLineArgumentChecker(const int argc, char ** argv) {
    _checkCommandLineArgumentCount(argc);
    _checkFirstCommandLineArgumentValue(argv[1]);
    _checkSecondCommandLineArgumentValue(argv[2]);
}

void CommandLineArgumentChecker::_checkCommandLineArgumentCount(const int argc) {
    if (argc < minArgc || argc > maxArgc) {
        throw std::invalid_argument("This program requires either two or three arguments.");
    }
}

void CommandLineArgumentChecker::_checkFirstCommandLineArgumentValue(const char * argv_1) {
    std::regex regex(argv_1_Regex);
    if (!std::regex_match(argv_1, regex)) {
        throw std::invalid_argument("The first command line argument of this program is wrong.");
    }
}

void CommandLineArgumentChecker::_checkSecondCommandLineArgumentValue(const char * argv_2) {
    if (argv_2) {
        if (strcmp(argv_2, argv_2_Regex)) {
            throw std::invalid_argument("The second command line argument of this program is wrong.");
        }
    }
}
