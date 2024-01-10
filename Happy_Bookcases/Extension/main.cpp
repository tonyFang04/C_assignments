#include <iostream>
#include "Parsing/misc.hpp"


int main(int argc, char ** argv) {
    try {
        CommandLineArgumentChecker(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
