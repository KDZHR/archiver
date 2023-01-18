#include "binary_processing_tools.h"

std::string GetFileName(const std::string& path) {
    size_t begin = 0;
    for (size_t index = 0; index < path.size(); ++index) {
        if (path[index] == '/') {
            begin = index + 1;
        }
    }
    return {std::next(path.begin(), static_cast<int64_t>(begin)), path.end()};
}

NodeSearcher::NodeSearcher(const std::vector<CodeRelation>& codes)
    : root_(std::make_shared<Node>(INITIALIZE_CHARACTER)) {
    for (const auto& [symbol, code] : codes) {
        CreateNode(code, symbol);
    }
}
AlphabetType NodeSearcher::GetLeafValue(const std::function<bool()>& get_next) const {
    auto cur_node = root_;
    while (!cur_node->IsLeaf()) {
        auto& next_node = (get_next() ? cur_node->right : cur_node->left);
        if (!next_node) {
            throw IncorrectTriePathException();
        }
        cur_node = next_node;
    }
    return cur_node->value;
}
void NodeSearcher::CreateNode(const CodeType& code, const AlphabetType& symbol) {
    auto cur_node = root_;
    for (size_t index = 0; index < code.Size(); ++index) {
        auto& next_node = (code.GetValue(index) ? cur_node->right : cur_node->left);
        if (!next_node) {
            next_node = std::make_shared<Node>(INITIALIZE_CHARACTER);
        }
        cur_node = next_node;
    }
    cur_node->value = symbol;
}
BinaryCode::BinaryCode() : data_(0), count_of_bits_(0) {
}
bool BinaryCode::GetValue(size_t index) const {
    return (data_ >> GetBitIndex(index)) & 1;
}
size_t BinaryCode::Size() const {
    return count_of_bits_;
}
void BinaryCode::PushBack(bool elem) {
    ++count_of_bits_;
    data_ <<= 1;
    data_ |= elem;
}
void BinaryCode::PopBack() {
    --count_of_bits_;
    data_ >>= 1;
}
void BinaryCode::operator++() {
    ++data_;
}
size_t BinaryCode::GetBitIndex(size_t index) const {
    return count_of_bits_ - index - 1;
}
void BinaryCode::WriteInBitStream(BitWriter& bit_writer) const {
    for (size_t index = 0; index < count_of_bits_; ++index) {
        bit_writer.WriteBit(GetValue(index));
    }
}
