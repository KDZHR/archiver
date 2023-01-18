#pragma once

#include <exception>
#include <string>

class ExceptionWithDescription : public std::exception {
public:
    explicit ExceptionWithDescription(const std::string& error_description);
    const char* what() const noexcept override;

protected:
    std::string error_description_;
};

class IncorrectConsoleArgumentsException : public ExceptionWithDescription {
public:
    explicit IncorrectConsoleArgumentsException();
};

class IncorrectFlagsException : public ExceptionWithDescription {
public:
    explicit IncorrectFlagsException();
};

class LessArgumentsThanRequiredException : public ExceptionWithDescription {
public:
    explicit LessArgumentsThanRequiredException();
};

class UnableToOpenTheFileException : public ExceptionWithDescription {
public:
    explicit UnableToOpenTheFileException(const std::string& filename);
};

class CreatingArchiveException : public ExceptionWithDescription {
public:
    explicit CreatingArchiveException(const std::string& message = "No specific information");
};

class BrokenInputFileException : public ExceptionWithDescription {
public:
    explicit BrokenInputFileException();
};

class EOFHasBeenReachedException : public ExceptionWithDescription {
public:
    explicit EOFHasBeenReachedException();
};

class IncorrectHuffmanTrieException : public ExceptionWithDescription {
public:
    explicit IncorrectHuffmanTrieException();
};

class ReadingArchiveException : public ExceptionWithDescription {
public:
    explicit ReadingArchiveException(const std::string& message = "No specific information");
};

class IncorrectTriePathException : public ExceptionWithDescription {
public:
    explicit IncorrectTriePathException();
};
