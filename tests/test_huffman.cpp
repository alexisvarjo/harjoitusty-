#include <gtest/gtest.h>

#include <random>
#include <unordered_map>
#include <string>
#include <iostream>

#include "../src/huffman.h"
#include "../src/utils.h"


// apufunktio satunnaisten stringien generoimiseen
#include <random>
#include <string>

std::string generate_random_string(size_t length) {
    // You can expand this with any UTF-8 characters you like.
    // Right now, these are basic ASCII letters and digits (still valid UTF-8).
    static const char* charset =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "öäåÖÄÅ"
        "0123456789";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, std::char_traits<char>::length(charset) - 1);

    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result.push_back(charset[dist(gen)]);
    }

    return result;
}


// testi, jossa testataan onko syötetty teksti sama kuin pakkauksen jälkeen purettu teksti
TEST(RandomInputTest, FunctionReturnsCorrectOutput) {
    const size_t test_count = 1;  // kuinka monta syötettä
    const size_t max_length = 1000;  // maksimipituus

    for (size_t i = 0; i < test_count; ++i) {
        std::string random_string = generate_random_string(max_length);
        EXPECT_EQ(huffman_decode(huffman_encode(random_string)), random_string);
    }
}

TEST(GetFrequenciesTest, NormalInputs) {
    std::string test_string1 = "testi";
    std::string test_string2 = "aakkoset toimii";
    std::string test_string3 = "hello world";
    std::string test_string4 = "1234567890";
    std::string test_string5 = "punctuation!@#";

    std::unordered_map<char, int> frequencies1 = {{'t', 2}, {'e', 1}, {'s', 1}, {'i', 1}};
    std::unordered_map<char, int> frequencies2 = {{'a', 2}, {'k', 2}, {'o', 2}, {'e', 1}, {'s', 1}, {'t', 2}, {'i', 3}, {' ', 1}, {'m', 1}, {'o', 1}};
    std::unordered_map<char, int> frequencies3 = {{'h', 1}, {'e', 1}, {'l', 3}, {'o', 2}, {' ', 1}, {'w', 1}, {'r', 1}, {'d', 1}};
    std::unordered_map<char, int> frequencies4 = {{'1', 1}, {'2', 1}, {'3', 1}, {'4', 1}, {'5', 1}, {'6', 1}, {'7', 1}, {'8', 1}, {'9', 1}, {'0', 1}};
    std::unordered_map<char, int> frequencies5 = {{'p', 1}, {'u', 2}, {'n', 2}, {'c', 1}, {'t', 2}, {'a', 1}, {'i', 1}, {'o', 1}, {'!', 1}, {'@', 1}, {'#', 1}};

    EXPECT_EQ(get_frequencies(test_string1), frequencies1);
    EXPECT_EQ(get_frequencies(test_string2), frequencies2);
    EXPECT_EQ(get_frequencies(test_string3), frequencies3);
    EXPECT_EQ(get_frequencies(test_string4), frequencies4);
    EXPECT_EQ(get_frequencies(test_string5), frequencies5);
}

// Test cases
TEST(EncodeTest, SingleCharTest) {
    std::string test_string = "a";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    EXPECT_EQ(encode(test_string, tree), "0");
}

TEST(EncodeTest, AllSameCharactersTest) {
    std::string test_string = "aaa";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    EXPECT_EQ(encode(test_string, tree), "000");
}

TEST(EncodeTest, MixedWithSpaceTest) {
    std::string test_string = "a b c d e f g h i j k l";
    // Ensure space is treated as a character

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));

    EXPECT_EQ(encode(test_string, tree), "1000010010111100111000110110101101010011111011010011101011001011000");
}

TEST(EncodeTest, LongerStringTest) {
    std::string test_string = "abcdefghijklmnopqrstuvwxyz ";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));

    // Expected encode for 26 letters + space
    EXPECT_EQ(encode(test_string, tree), "1100110100001000111101000000001111001101110001101011100001000110110111101100111101110100001001101100111111110110010111100101101010");
}

TEST(TreeSerializationTest, SingleNodeTree) {
    Node* root = new Node{'a', 1};
    EXPECT_EQ(serializeTree(root), "1a");
    delete root;
}

TEST(TreeSerializationTest, SimpleTree) {
    Node* left = new Node{'b', 1};
    Node* right = new Node{'c', 1};
    Node* root = new Node{2, left, right};
    EXPECT_EQ(serializeTree(root), "01b1c");
    delete left;
    delete right;
    delete root;
}

TEST(TreeSerializationTest, ComplexTree) {
    Node* left_left = new Node{'d', 1};
    Node* left_right = new Node{'e', 1};
    Node* right_left = new Node{'f', 1};
    Node* right_right = new Node{'g', 1};
    Node* left = new Node{2, left_left, left_right};
    Node* right = new Node{2, right_left, right_right};
    Node* root = new Node{4, left, right};
    EXPECT_EQ(serializeTree(root), "001d1e01f1g");
    delete left_left;
    delete left_right;
    delete right_left;
    delete right_right;
    delete left;
    delete right;
    delete root;
}

TEST(TreeSerializationTest, EmptyTree) {
    EXPECT_EQ(serializeTree(nullptr), "");
}

TEST(TreeDeserializationTest, SingleNodeTree) {
    std::string serialized = "1a";
    HuffmanTree tree;
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    Node* child = root->left;
    EXPECT_EQ(root->right, nullptr);
    EXPECT_EQ(root->left, nullptr);
    EXPECT_EQ(root->symbol, 'a');
}

TEST(DataStripTest, SimpleTreeAndText) {
    // We want treeSize = 2 (binary) and bitCount = 8 (binary).

    // 1) Make a 16-byte header (8 bytes for treeSize + 8 bytes for bitCount).
    std::string raw_data(16, '\0');

    // treeSize = 2 in little-endian => put a 2 in raw_data[0].
    raw_data[0] = static_cast<char>(2);

    // bitCount = 8 in little-endian => put an 8 in raw_data[8].
    raw_data[8] = static_cast<char>(8);

    // 2) Now append the actual tree data (2 bytes) plus whatever remains
    raw_data += "1a";            // tree part (2 bytes)
    raw_data += "encodedtext";   // remainder

    // Now call stripData
    auto result = stripData(raw_data);

    EXPECT_EQ(std::get<0>(result), "1a");
    EXPECT_EQ(std::get<1>(result), 8);
    EXPECT_EQ(std::get<2>(result), "encodedtext");
}


TEST(DataStripTest, EmptyTreeAndText) {
    // We want:
    //   treeSize = 0 (64-bit little-endian)
    //   bitCount = 0 (64-bit little-endian)
    //   remainder = "encodedtext"

    // Create 16 bytes of header (8 bytes for treeSize, 8 for bitCount).
    std::string raw_data(16, '\0');

    // treeSize = 0 => all 8 bytes are already '\0' (no changes needed).
    // bitCount = 0 => likewise remains '\0'.

    // Append the remainder ("encodedtext").
    raw_data += "encodedtext";

    // Now call stripData with this binary header.
    auto result = stripData(raw_data);

    EXPECT_EQ(std::get<0>(result), "");              // Empty tree
    EXPECT_EQ(std::get<1>(result), 0u);              // Bit count = 0
    EXPECT_EQ(std::get<2>(result), "encodedtext");   // The remaining data
}

TEST(DataStripTest, ComplexTreeAndText) {
    // We want:
    //   treeSize = 13 (binary)
    //   bitCount = 12 (binary)
    //   tree string = "0010d1e010f1g" (13 bytes)
    //   remainder = "encodedtext"

    // 1) Make a 16-byte header (8 bytes for treeSize, 8 for bitCount).
    std::string raw_data(16, '\0');

    // treeSize = 13 in little-endian => put char(13) at raw_data[0].
    raw_data[0] = static_cast<char>(13);

    // bitCount = 12 in little-endian => put char(12) at raw_data[8].
    raw_data[8] = static_cast<char>(12);

    // 2) Append the 13-byte tree data.
    //    (Make sure it’s exactly 13 bytes long!)
    raw_data += "0010d1e010f1g";

    // 3) Append the remainder
    raw_data += "encodedtext";

    auto result = stripData(raw_data);

    EXPECT_EQ(std::get<0>(result), "0010d1e010f1g");  // The 13-byte tree
    EXPECT_EQ(std::get<1>(result), 12u);              // bitCount = 12
    EXPECT_EQ(std::get<2>(result), "encodedtext");    // remainder
}


TEST(DecodeTest, SingleCharTest) {
    std::string test_string = "a";
    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(DecodeTest, AllSameCharactersTest) {
    std::string test_string = "aaa";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(DecodeTest, MixedWithSpaceTest) {
    std::string test_string = "a b c d e f g h i j k l";
    // Ensure space is treated as a character

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(DecodeTest, LongerStringTest) {
    std::string test_string = "abcdefghijklmnopqrstuvwxyz ";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(HuffmanEncodeTest, EmptyString) {
    std::string test_string = "";
    testing::internal::CaptureStderr();

    std::string result = huffman_encode(test_string);

    std::string stderr_output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(result, "");
    EXPECT_EQ(stderr_output, "Error: Empty string\n");
}

TEST(HuffmanEncodeTest, SingleChar) {
    std::string test_string = "a";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, AllSameCharacters) {
    std::string test_string = "aaa";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, MixedWithSpace) {
    std::string test_string = "a b c d e f g h i j k l";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, LongerString) {
    std::string test_string = "abcdefghijklmnopqrstuvwxyz ";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, ComplexString) {
    std::string test_string = "The quick brown fox jumps over the lazy dog 1234567890!@#";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, EmptyString) {
    std::string bitstream = "";
    EXPECT_EQ(huffman_decode(bitstream), "");
}

TEST(HuffmanDecodeTest, SingleChar) {
    std::string test_string = "a";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, AllSameCharacters) {
    std::string test_string = "aaa";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, MixedWithSpace) {
    std::string test_string = "a b c d e f g h i j k l";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, LongerString) {
    std::string test_string = "abcdefghijklmnopqrstuvwxyz ";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, ComplexString) {
    std::string test_string = "The quick brown fox jumps over the lazy dog 1234567890!@#";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanTreeTest, GenerateCodesTest) {
    HuffmanTree tree;
    std::unordered_map<char, int> frequencies = {{'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}};
    tree.build(frequencies);
    std::unordered_map<char, std::string> codes;
    codes = tree.getCodes();
    EXPECT_EQ(codes['a'], "1100");
    EXPECT_EQ(codes['b'], "1101");
    EXPECT_EQ(codes['c'], "100");
    EXPECT_EQ(codes['d'], "101");
    EXPECT_EQ(codes['e'], "111");
    EXPECT_EQ(codes['f'], "0");
}

TEST(HuffmanTreeTest, GenerateCodesTestWithSingleCharacter) {
    HuffmanTree tree;
    std::unordered_map<char, int> frequencies = {{'a', 1}};
    tree.build(frequencies);
    std::unordered_map<char, std::string> codes;
    codes = tree.getCodes();
    EXPECT_EQ(codes['a'], "0");
}

TEST(HuffmanTreeTest, GenerateCodesTestWithEqualFrequencies) {
    HuffmanTree tree;
    std::unordered_map<char, int> frequencies = {{'a', 1}, {'b', 1}, {'c', 1}, {'d', 1}};
    tree.build(frequencies);
    std::unordered_map<char, std::string> codes;
    codes = tree.getCodes();
    EXPECT_EQ(codes.size(), 4);
    EXPECT_TRUE(codes['a'].length() > 0);
    EXPECT_TRUE(codes['b'].length() > 0);
    EXPECT_TRUE(codes['c'].length() > 0);
    EXPECT_TRUE(codes['d'].length() > 0);
}

TEST(HuffmanTreeTest, BuildFromSerializedTreeTest) {
    HuffmanTree tree;
    std::string serialized = "001d1e01f1g";
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->left->right->symbol, 'e');
    EXPECT_EQ(root->right->left->symbol, 'f');
    EXPECT_EQ(root->right->right->symbol, 'g');
}

TEST(HuffmanTreeTest, BuildFromSerializedTreeSingleNodeTest) {
    HuffmanTree tree;
    std::string serialized = "1a";
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->symbol, 'a');
    EXPECT_EQ(root->left, nullptr);
    EXPECT_EQ(root->right, nullptr);
}

TEST(HuffmanTreeTest, BuildFromSerializedTreeComplexTest) {
    HuffmanTree tree;
    std::string serialized = "001d1e01f01h1i";
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->left->left->symbol, 'd');
    EXPECT_EQ(root->left->right->symbol, 'e');
    EXPECT_EQ(root->right->left->symbol, 'f');
    EXPECT_EQ(root->right->right->left->symbol, 'h');
    EXPECT_EQ(root->right->right->right->symbol, 'i');
}

TEST(HuffmanTreeTest, ConstructorTest) {
    HuffmanTree tree;
    EXPECT_EQ(tree.getRoot(), nullptr);
}

TEST(HuffmanTreeTest, ConstructorWithFrequenciesTest) {
    HuffmanTree tree;
    std::unordered_map<char, int> frequencies = {{'a', 1}, {'b', 2}};
    tree.build(frequencies);
    EXPECT_NE(tree.getRoot(), nullptr);
}

TEST(HuffmanTreeTest, ConstructorWithSingleCharacterTest) {
    HuffmanTree tree;
    std::unordered_map<char, int> frequencies = {{'a', 1}};
    tree.build(frequencies);
    EXPECT_NE(tree.getRoot(), nullptr);
    EXPECT_EQ(tree.getRoot()->symbol, 'a');
}

TEST(HuffmanTreeTest, DestructorTest) {
    HuffmanTree* tree = new HuffmanTree();
    delete tree;
    // No explicit check here, just ensuring no crash on delete
}

TEST(HuffmanTreeTest, DestructorWithNodesTest) {
    HuffmanTree* tree = new HuffmanTree();
    std::unordered_map<char, int> frequencies = {{'a', 1}, {'b', 2}};
    tree->build(frequencies);
    delete tree;
    // No explicit check here, just ensuring no crash on delete
}

TEST(HuffmanTreeTest, DestructorWithEmptyTreeTest) {
    HuffmanTree* tree = new HuffmanTree();
    delete tree;
    // No explicit check here, just ensuring no crash on delete
}

TEST(HuffmanTreeTest, DestructorWithSingleCharacterTest) {
    HuffmanTree* tree = new HuffmanTree();
    std::unordered_map<char, int> frequencies = {{'a', 1}};
    tree->build(frequencies);
    delete tree;
    // No explicit check here, just ensuring no crash on delete
}

TEST(HuffmanTreeTest, BuildTest) {
    HuffmanTree tree;
    std::unordered_map<char, int> frequencies = {{'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}};
    tree.build(frequencies);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->frequency, 100);
    EXPECT_EQ(root->left->frequency, 45);
    EXPECT_EQ(root->right->frequency, 55);
}
