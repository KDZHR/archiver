#pragma once

#include <bitset>
#include <fstream>
#include <vector>

namespace {
const size_t STREAM_GET_SIZE = 8;
const size_t STREAM_WRITE_SIZE = 8;
const size_t READER_CHAR_BUFFER_SIZE = 1024;
const size_t WRITER_CHAR_BUFFER_SIZE = 1024;
}  // namespace

class BitReader {
public:
    explicit BitReader(std::istream& istream);

    std::vector<bool> ReadNBits(size_t count);

private:
    bool ReadBit();

    std::istream& istream_;
    std::bitset<STREAM_GET_SIZE> bit_buffer_;
    size_t bits_count_;
    std::string char_buffer_;
    size_t bytes_count_;
    size_t first_byte_;
};

class BitWriter {
public:
    explicit BitWriter(std::ostream& ostream);

    void WriteBit(bool value);
    void WriteNBits(const std::vector<bool>& bits);
    void WriteBufferAndClean();
    ~BitWriter();

private:
    std::ostream& ostream_;
    std::bitset<STREAM_WRITE_SIZE> bit_buffer_;
    size_t bits_count_;
    std::string char_buffer_;
    size_t first_byte_;
};