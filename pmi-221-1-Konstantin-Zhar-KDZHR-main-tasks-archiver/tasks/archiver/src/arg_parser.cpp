#include "arg_parser.h"

ArgParser::ArgParser(int argc, char** argv) {
    bool passed_flags = false;
    for (int index = 1; index < argc; ++index) {
        if (argv[index][0] == '-') {
            if (passed_flags) {
                throw IncorrectConsoleArgumentsException();
            }
            flags_.insert(argv[index]);
        } else {
            passed_flags = true;
            arguments_.push_back(argv[index]);
        }
    }
}
bool ArgParser::HasFlag(const std::string& flag) const {
    return flags_.contains(flag);
}
size_t ArgParser::FlagsCount() const {
    return flags_.size();
}
size_t ArgParser::ArgumentsCount() const {
    return arguments_.size();
}
const std::unordered_set<std::string>& ArgParser::GetFlags() const {
    return flags_;
}
const std::vector<std::string>& ArgParser::GetArguments() const {
    return arguments_;
}
