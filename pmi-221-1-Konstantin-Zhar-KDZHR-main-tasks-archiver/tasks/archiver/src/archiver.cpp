#include <iostream>

#include "arg_parser.h"

int main(int argc, char** argv) {
    try {
        auto parser = ArgParser(argc, argv);
        if (parser.FlagsCount() != 1) {
            throw IncorrectFlagsException();
        }
        if (parser.HasFlag("-c")) {
            if (parser.ArgumentsCount() < 2) {
                throw LessArgumentsThanRequiredException();
            }
            CompressFiles(parser.GetArguments());
        } else if (parser.HasFlag("-d")) {
            if (parser.ArgumentsCount() < 1) {
                throw LessArgumentsThanRequiredException();
            }
            DecompressFiles(parser.GetArguments());
        } else if (parser.HasFlag("-h")) {
            PrintHelpMessage();
        } else {
            throw IncorrectFlagsException();
        }
    } catch (const std::exception& exc) {
        std::cout << exc.what();
        return 111;
    } catch (...) {
        std::cout << "UNEXPECTED ERROR";
        return 111;
    }
}
