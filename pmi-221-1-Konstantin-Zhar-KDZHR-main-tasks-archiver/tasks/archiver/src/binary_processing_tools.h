#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

#include "bit_streams.h"
#include "exceptions.h"
#include "trie.h"

class BinaryCode {
public:
    BinaryCode();

    bool GetValue(size_t index) const;
    size_t Size() const;
    void PushBack(bool elem);
    void PopBack();
    void WriteInBitStream(BitWriter& bit_writer) const;

    void operator++();

private:
    size_t GetBitIndex(size_t index) const;

    uint64_t data_;
    size_t count_of_bits_;
};

using AlphabetType = uint16_t;
using Node = TrieNode<AlphabetType>;
using CodeType = BinaryCode;
using CodeRelation = std::pair<AlphabetType, CodeType>;
using FrequenciesType = std::unordered_map<AlphabetType, size_t>;
const size_t READ_CNT = 8;

const size_t WRITE_CNT = 9;
const AlphabetType FILENAME_END = 256;
const AlphabetType ONE_MORE_FILE = 257;
const AlphabetType ARCHIVE_END = 258;
const AlphabetType MIN_SPECIAL_CHARACTER = 256;
const AlphabetType INITIALIZE_CHARACTER = 300;

template <typename T>
T GetNumberBE(const std::vector<bool>& bool_vec) {
    T number = 0;
    for (bool bin : bool_vec) {
        number <<= 1;
        number |= bin;
    }
    return number;
}

template <typename T>
std::vector<bool> GetNBitsBE(const T& number, size_t count) {
    std::vector<bool> bits(count);
    if (count == 0) {
        return bits;
    }
    for (size_t shift = 0; shift < count; ++shift) {
        bits[count - shift - 1] = (static_cast<uint64_t>(number) >> shift) & 1;
    }
    return bits;
}

std::string GetFileName(const std::string& path);

class NodeSearcher {
public:
    explicit NodeSearcher(const std::vector<CodeRelation>& codes);
    AlphabetType GetLeafValue(const std::function<bool()>& get_next) const;

private:
    void CreateNode(const CodeType& code, const AlphabetType& symbol);
    std::shared_ptr<Node> root_;
};
