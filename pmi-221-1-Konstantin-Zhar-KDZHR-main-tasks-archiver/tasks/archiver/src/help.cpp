#include "help.h"

#include <iostream>

void PrintHelpMessage() {
    std::cout << R"(archiver is a simple console utility that can compress files using Huffman coding
OPTIONS:
    archiver -c archive_name file1 [file2 ...]      Creates archive_name in current working directory
                                                    which contains compressed file1 [file2 ...]
    archiver -d archive_name                        Extracts compressed files to current working directory
                                                    from archive_name
    archiver -h                                     Prints help message
)";
}
