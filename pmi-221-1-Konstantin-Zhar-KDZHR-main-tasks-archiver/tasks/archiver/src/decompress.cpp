#include "decompress.h"

#include "binary_processing_tools.h"
#include "bit_streams.h"
#include "exceptions.h"

std::vector<CodeRelation> ReadCodes(BitReader& bit_reader) {
    size_t alphabet_size = GetNumberBE<AlphabetType>(bit_reader.ReadNBits(WRITE_CNT));
    std::vector<CodeRelation> codes(alphabet_size);
    for (size_t symbol_index = 0; symbol_index < alphabet_size; ++symbol_index) {
        codes[symbol_index].first = GetNumberBE<AlphabetType>(bit_reader.ReadNBits(WRITE_CNT));
    }
    size_t cur_codes_count = 0;
    CodeType cur_code;
    size_t least_unknown_code_index = 0;

    for (size_t codes_length = 1; cur_codes_count < alphabet_size; ++codes_length) {
        size_t codes_with_fixed_length_count = GetNumberBE<AlphabetType>(bit_reader.ReadNBits(WRITE_CNT));
        if (codes_with_fixed_length_count != 0) {
            cur_codes_count += codes_with_fixed_length_count;
            while (cur_code.Size() < codes_length) {
                cur_code.PushBack(false);
            }
            for (size_t code_index = 0; code_index < codes_with_fixed_length_count; ++code_index) {
                codes[least_unknown_code_index++].second = cur_code;
                ++cur_code;
            }
        }
    }
    return codes;
}

// true if ARCHIVE_END has been reached and false if it's ONE_MORE_FILE. Otherwise, throws BrokenInputFileException
bool CreateDecompressedFile(BitReader& bit_reader, const std::vector<CodeRelation>& codes) {
    NodeSearcher searcher(codes);
    std::string filename;
    AlphabetType cur_symbol = INITIALIZE_CHARACTER;
    auto get_single_bit = [&bit_reader]() {
        auto bits = bit_reader.ReadNBits(1);
        if (bits.empty()) {
            throw ReadingArchiveException();
        } else {
            return static_cast<bool>(bits[0]);
        }
    };
    while (true) {
        cur_symbol = searcher.GetLeafValue(get_single_bit);
        if (cur_symbol >= MIN_SPECIAL_CHARACTER) {
            if (cur_symbol == FILENAME_END) {
                break;
            } else {
                throw BrokenInputFileException();
            }
        }
        filename.push_back(static_cast<char>(cur_symbol));
    }
    std::ofstream ofstream(filename, std::ios::binary);
    BitWriter bit_writer(ofstream);
    while (true) {
        cur_symbol = searcher.GetLeafValue(get_single_bit);
        if (cur_symbol >= MIN_SPECIAL_CHARACTER) {
            if (cur_symbol == ARCHIVE_END) {
                return true;
            }
            if (cur_symbol == ONE_MORE_FILE) {
                return false;
            }
            throw BrokenInputFileException();
        }
        bit_writer.WriteNBits(GetNBitsBE(cur_symbol, READ_CNT));
    }
}

void DecompressFiles(const std::vector<std::string>& arguments) {
    std::string archive_path = arguments[0];
    std::string archive_name = GetFileName(archive_path);

    try {
        std::ifstream input_stream(archive_path, std::ios::binary);
        auto bit_reader = BitReader(input_stream);
        while (true) {
            if (CreateDecompressedFile(bit_reader, ReadCodes(bit_reader))) {
                break;
            }
        }
    } catch (const std::exception& exception) {
        throw ReadingArchiveException(exception.what());
    } catch (...) {
        throw ReadingArchiveException();
    }
}
