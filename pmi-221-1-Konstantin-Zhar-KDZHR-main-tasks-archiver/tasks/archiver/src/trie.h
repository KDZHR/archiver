#pragma once

#include <memory>

template <typename T>
struct TrieNode {
    using PtrType = std::shared_ptr<TrieNode>;
    explicit TrieNode(const T& value, const PtrType& left = nullptr, const PtrType& right = nullptr)
        : value(value), left(left), right(right) {
    }
    T value;
    PtrType left;
    PtrType right;

    bool IsLeaf() const {
        return !left && !right;
    }
};
