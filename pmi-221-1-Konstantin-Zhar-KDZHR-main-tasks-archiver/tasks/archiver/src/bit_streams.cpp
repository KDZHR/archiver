#include "bit_streams.h"

#include "exceptions.h"

BitReader::BitReader(std::istream& istream)
    : istream_(istream), bits_count_(0), char_buffer_(READER_CHAR_BUFFER_SIZE, '?'), bytes_count_(0), first_byte_(0) {
}
bool BitReader::ReadBit() {
    if (bits_count_ == 0) {
        if (first_byte_ == bytes_count_) {
            istream_.read(char_buffer_.data(), READER_CHAR_BUFFER_SIZE);
            bytes_count_ = istream_.gcount();
            if (bytes_count_ == 0) {
                throw EOFHasBeenReachedException();
            }
            first_byte_ = 0;
        }
        bit_buffer_ = char_buffer_[first_byte_++];
        bits_count_ = STREAM_GET_SIZE;
    }
    bool value = bit_buffer_[STREAM_GET_SIZE - 1];
    bit_buffer_ <<= 1;
    --bits_count_;
    return value;
}
std::vector<bool> BitReader::ReadNBits(size_t count) {
    std::vector<bool> bits;
    for (size_t index = 0; index < count; ++index) {
        try {
            bits.push_back(ReadBit());
        } catch (const EOFHasBeenReachedException& exception) {
            break;
        }
    }
    return bits;
}

BitWriter::BitWriter(std::ostream& ostream)
    : ostream_(ostream), bits_count_(0), char_buffer_(WRITER_CHAR_BUFFER_SIZE, '?'), first_byte_(0) {
}
void BitWriter::WriteBit(bool value) {
    if (bits_count_ == STREAM_WRITE_SIZE) {
        if (first_byte_ == WRITER_CHAR_BUFFER_SIZE) {
            WriteBufferAndClean();
        }
        char_buffer_[first_byte_++] = static_cast<char>(bit_buffer_.to_ulong());
        bits_count_ = 0;
    }
    bit_buffer_ <<= 1;
    bit_buffer_[0] = value;
    ++bits_count_;
}
void BitWriter::WriteNBits(const std::vector<bool>& bits) {
    for (bool value : bits) {
        WriteBit(value);
    }
}
void BitWriter::WriteBufferAndClean() {
    ostream_.write(char_buffer_.data(), static_cast<std::streamsize>(first_byte_));
    first_byte_ = 0;
}
BitWriter::~BitWriter() {
    if (bits_count_ == 0) {
        return;
    }
    for (size_t i = bits_count_; i <= STREAM_WRITE_SIZE; ++i) {
        WriteBit(false);
    }
    WriteBufferAndClean();
}
