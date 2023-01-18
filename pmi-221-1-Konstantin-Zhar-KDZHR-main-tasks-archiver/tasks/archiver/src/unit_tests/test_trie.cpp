#include <catch.hpp>
#include <queue>
#include <string>

#include "../trie.h"

using StringNode = TrieNode<std::string>;

TEST_CASE("Trie") {
    auto left_right = std::make_shared<StringNode>("left_right");
    auto left = std::make_shared<StringNode>("left", nullptr, left_right);

    auto right_left = std::make_shared<StringNode>("right_left");
    auto right = std::make_shared<StringNode>("right", right_left);

    auto root = std::make_shared<StringNode>("root", left, right);

    REQUIRE(root->left->value == "left");
    REQUIRE(root->right->left->value == "right_left");

    REQUIRE(root->right->left->IsLeaf());
    REQUIRE(!root->left->IsLeaf());
    REQUIRE(!root->IsLeaf());
    REQUIRE(!root->right->IsLeaf());
}
