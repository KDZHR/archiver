#include "exceptions.h"

ExceptionWithDescription::ExceptionWithDescription(const std::string &error_description)
    : error_description_(error_description) {
}
const char *ExceptionWithDescription::what() const noexcept {
    return error_description_.c_str();
}

IncorrectConsoleArgumentsException::IncorrectConsoleArgumentsException()
    : ExceptionWithDescription("Given arguments are incorrect") {
}
IncorrectFlagsException::IncorrectFlagsException() : ExceptionWithDescription("Given flags are incorrect") {
}
LessArgumentsThanRequiredException::LessArgumentsThanRequiredException()
    : ExceptionWithDescription("Given number of arguments is less than required one\n") {
}
UnableToOpenTheFileException::UnableToOpenTheFileException(const std::string &filename)
    : ExceptionWithDescription("Unable to open the file " + filename + "\n") {
}
CreatingArchiveException::CreatingArchiveException(const std::string &message)
    : ExceptionWithDescription("Error while creating the archive: " + message) {
}
BrokenInputFileException::BrokenInputFileException() : ExceptionWithDescription("One of input files is broken") {
}
EOFHasBeenReachedException::EOFHasBeenReachedException()
    : ExceptionWithDescription("EOF of file has been reached unexpectedly") {
}
IncorrectHuffmanTrieException::IncorrectHuffmanTrieException()
    : ExceptionWithDescription("Trie has been made incorrectly") {
}
ReadingArchiveException::ReadingArchiveException(const std::string &message)
    : ExceptionWithDescription("Error while reading the archive: " + message) {
}
IncorrectTriePathException::IncorrectTriePathException() : ExceptionWithDescription("Incorrect trie path") {
}
