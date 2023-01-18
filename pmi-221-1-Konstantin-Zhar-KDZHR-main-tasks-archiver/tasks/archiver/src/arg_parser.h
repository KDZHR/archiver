#pragma once

#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

#include "compress.h"
#include "decompress.h"
#include "exceptions.h"
#include "help.h"

class ArgParser {
public:
    ArgParser(int argc, char** argv);

    bool HasFlag(const std::string& flag) const;
    size_t FlagsCount() const;
    size_t ArgumentsCount() const;
    const std::unordered_set<std::string>& GetFlags() const;
    const std::vector<std::string>& GetArguments() const;

private:
    std::unordered_set<std::string> flags_;
    std::vector<std::string> arguments_;
};
