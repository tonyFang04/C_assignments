#include <stdexcept>
#include <string.h>
#include <regex>
#include <string>
#define MAXHEIGHT 9
#define MAXWIDTH 9


// struct Bookcase {
//     int height, width;

// };

class CommandLineArgumentChecker {
    const int minArgc = 2;
    const int maxArgc = 3;
    const char * argv_1_Regex = "^.*\\.bc$";
    const char * argv_2_Regex = "verbose";
    public:
        CommandLineArgumentChecker(const int argc, char ** argv);
        void displayOutput();
        void _checkCommandLineArgumentCount(const int argc);
        void _checkFirstCommandLineArgumentValue(const char * argv_1);
        void _checkSecondCommandLineArgumentValue(const char * argv_2);
};

// class FileParser {
//     public:
//         FileParser(const int argc, char ** argv);

// };
