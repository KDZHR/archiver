#include "compress.h"

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>

#include "binary_processing_tools.h"
#include "priority_queue.h"

void OpenInputFileStreams(const std::vector<std::string>& filenames, std::vector<std::ifstream>& input_streams) {
    input_streams.reserve(filenames.size());
    for (const auto& filename : filenames) {
        try {
            input_streams.emplace_back(filename, std::ios::binary);
        } catch (...) {
            throw UnableToOpenTheFileException(filename);
        }
    }
}
void CountFrequenciesInStream(std::istream& input_stream, FrequenciesType& frequencies) {
    auto reader = BitReader(input_stream);
    while (true) {
        auto bits = reader.ReadNBits(READ_CNT);
        if (bits.empty()) {
            break;
        }
        if (bits.size() != READ_CNT) {
            throw BrokenInputFileException();
        }
        ++frequencies[GetNumberBE<AlphabetType>(bits)];
    }
}
void CountFrequenciesInString(const std::string& string, FrequenciesType& frequencies) {
    for (AlphabetType elem : string) {
        ++frequencies[elem];
    }
}

void TrieDFS(std::shared_ptr<Node> cur_node, CodeType& cur_code, std::vector<CodeRelation>& huffman_code) {
    if ((cur_node->left == nullptr) ^ (cur_node->right == nullptr)) {
        throw IncorrectHuffmanTrieException();
    }
    if (cur_node->left == nullptr) {
        huffman_code.emplace_back(cur_node->value, cur_code);
    } else {
        cur_code.PushBack(false);
        TrieDFS(cur_node->left, cur_code, huffman_code);
        cur_code.PopBack();
        cur_code.PushBack(true);
        TrieDFS(cur_node->right, cur_code, huffman_code);
        cur_code.PopBack();
    }
}

struct QueueElement {
    size_t count;
    AlphabetType symbol;
    std::shared_ptr<Node> ptr;
};

bool CompareQueueElements(const QueueElement& first, const QueueElement& second) {
    if (first.count != second.count) {
        return first.count > second.count;
    }
    return first.symbol > second.symbol;
}

std::vector<CodeRelation> GetHuffmanCode(FrequenciesType& frequencies) {
    PriorityQueue<QueueElement> queue(CompareQueueElements);
    for (const auto& [symbol, count] : frequencies) {
        queue.Push({count, symbol, std::make_shared<Node>(symbol)});
    }
    while (queue.Size() > 1) {
        auto first_elem = queue.Pop();
        auto second_elem = queue.Pop();
        auto new_symbol = std::min(first_elem.ptr->value, second_elem.ptr->value);
        queue.Push({first_elem.count + second_elem.count, new_symbol,
                    std::make_shared<Node>(new_symbol, first_elem.ptr, second_elem.ptr)});
    }
    auto root_elem = queue.Pop();
    CodeType code;
    std::vector<CodeRelation> huffman_code;
    TrieDFS(root_elem.ptr, code, huffman_code);
    return huffman_code;
}

std::vector<std::pair<AlphabetType, CodeType>> GetCanonicalHuffmanCode(FrequenciesType& frequencies) {
    auto huffman_code = GetHuffmanCode(frequencies);
    std::sort(huffman_code.begin(), huffman_code.end(), [](const CodeRelation& a, const CodeRelation& b) {
        return std::tuple(a.second.Size(), a.first) < std::tuple(b.second.Size(), b.first);
    });
    CodeType cur_code;
    cur_code.PushBack(false);
    std::vector<std::pair<AlphabetType, CodeType>> canonical_code;
    bool first_element = true;
    for (const auto& [symbol, code] : huffman_code) {
        if (!first_element) {
            ++cur_code;
        } else {
            first_element = false;
        }
        while (cur_code.Size() < code.Size()) {
            cur_code.PushBack(false);
        }
        canonical_code.emplace_back(symbol, cur_code);
    }
    return canonical_code;
}

void WriteEncodedDataInArchive(std::ostream& archive, size_t files_count, const std::vector<std::string>& filepaths,
                               const std::vector<std::string>& filenames, std::vector<std::ifstream>& input_streams) {
    auto output_stream = BitWriter(archive);
    auto put_raw_number = [&output_stream]<typename T>(T number) {
        output_stream.WriteNBits(GetNBitsBE(number, WRITE_CNT));
    };
    for (size_t file_index = 0; file_index < files_count; ++file_index) {
        FrequenciesType frequencies;
        frequencies[FILENAME_END] = frequencies[ONE_MORE_FILE] = frequencies[ARCHIVE_END] = 1;
        CountFrequenciesInString(filenames[file_index], frequencies);
        CountFrequenciesInStream(input_streams[file_index], frequencies);
        auto codes = GetCanonicalHuffmanCode(frequencies);

        put_raw_number(codes.size());

        std::unordered_map<AlphabetType, CodeType> get_code;
        auto put_encoded_number = [&output_stream, &get_code](AlphabetType number) {
            get_code[number].WriteInBitStream(output_stream);
        };

        std::vector<size_t> size_count(codes.back().second.Size() + 1, 0);
        for (const auto& [symbol, code] : codes) {
            put_raw_number(symbol);
            get_code[symbol] = code;
            ++size_count[code.Size()];
        }
        for (size_t size = 1; size < size_count.size(); ++size) {
            put_raw_number(size_count[size]);
        }
        for (auto elem : filenames[file_index]) {
            put_encoded_number(elem);
        }
        put_encoded_number(FILENAME_END);
        input_streams[file_index].clear();
        input_streams[file_index].seekg(0, std::ios::beg);
        auto reader = BitReader(input_streams[file_index]);
        while (true) {
            auto bits = reader.ReadNBits(READ_CNT);
            if (bits.empty()) {
                break;
            }
            if (bits.size() != READ_CNT) {
                throw BrokenInputFileException();
            }
            put_encoded_number(GetNumberBE<AlphabetType>(bits));
        }
        if (file_index + 1 == files_count) {
            put_encoded_number(ARCHIVE_END);
        } else {
            put_encoded_number(ONE_MORE_FILE);
        }
    }
}

void CompressFiles(const std::vector<std::string>& arguments) {
    std::vector<std::ifstream> input_streams;
    size_t files_count = arguments.size() - 1;
    std::vector<std::string> filepaths(files_count);
    std::vector<std::string> filenames(files_count);
    for (size_t index = 1; index < arguments.size(); ++index) {
        filepaths[index - 1] = arguments[index];
        filenames[index - 1] = GetFileName(filepaths[index - 1]);
    }
    OpenInputFileStreams(filepaths, input_streams);

    const auto& archive_name = arguments[0];
    try {
        std::ofstream archive(archive_name, std::ios::binary);
        WriteEncodedDataInArchive(archive, files_count, filepaths, filenames, input_streams);
    } catch (const std::exception& exception) {
        throw CreatingArchiveException(exception.what());
    } catch (...) {
        throw CreatingArchiveException();
    }
}
