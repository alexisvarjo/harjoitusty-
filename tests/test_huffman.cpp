#include <gtest/gtest.h>

#include <random>
#include <unordered_map>
#include <string>
#include <iostream>

#include "../src/huffman.h"
#include "../src/utils.h"


// apufunktio satunnaisten u32stringien generoimiseen
std::u32string generate_random_u32string(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0x0000, 0x10FFFF);

    std::u32string random_string;
    for (size_t i = 0; i < length; ++i) {
        uint32_t random_char = dis(gen);

        // jättää pois tietyt ongelmalliset merkit (0xD800 - 0xDFFF)
        if (random_char >= 0xD800 && random_char <= 0xDFFF) {
            --i;
            continue;
        }
        random_string.push_back(static_cast<char32_t>(random_char));
    }
    return random_string;
}

// testi, jossa testataan onko syötetty teksti sama kuin pakkauksen jälkeen purettu teksti
TEST(RandomInputTest, FunctionReturnsCorrectOutput) {
    const size_t test_count = 1;  // kuinka monta syötettä
    const size_t max_length = 1000;  // maksimipituus

    for (size_t i = 0; i < test_count; ++i) {
        std::u32string random_string = generate_random_u32string(max_length);
        EXPECT_EQ(huffman_decode(huffman_encode(random_string)), random_string);
    }
}

TEST(GetFrequenciesTest, NormalInputs) {
    std::u32string test_string1 = U"testi";
    std::u32string test_string2 = U"ääkköset toimii";
    std::u32string test_string3 = U"hello world";
    std::u32string test_string4 = U"1234567890";
    std::u32string test_string5 = U"punctuation!@#";

    std::unordered_map<char32_t, int> frequencies1 = {{U't', 2}, {U'e', 1}, {U's', 1}, {U'i', 1}};
    std::unordered_map<char32_t, int> frequencies2 = {{U'ä', 2}, {U'k', 2}, {U'ö', 1}, {U'e', 1}, {U's', 1}, {U't', 2}, {U'i', 3}, {U' ', 1}, {U'm', 1}, {U'o', 1}};
    std::unordered_map<char32_t, int> frequencies3 = {{U'h', 1}, {U'e', 1}, {U'l', 3}, {U'o', 2}, {U' ', 1}, {U'w', 1}, {U'r', 1}, {U'd', 1}};
    std::unordered_map<char32_t, int> frequencies4 = {{U'1', 1}, {U'2', 1}, {U'3', 1}, {U'4', 1}, {U'5', 1}, {U'6', 1}, {U'7', 1}, {U'8', 1}, {U'9', 1}, {U'0', 1}};
    std::unordered_map<char32_t, int> frequencies5 = {{U'p', 1}, {U'u', 2}, {U'n', 2}, {U'c', 1}, {U't', 2}, {U'a', 1}, {U'i', 1}, {U'o', 1}, {U'!', 1}, {U'@', 1}, {U'#', 1}};

    EXPECT_EQ(get_frequencies(test_string1), frequencies1);
    EXPECT_EQ(get_frequencies(test_string2), frequencies2);
    EXPECT_EQ(get_frequencies(test_string3), frequencies3);
    EXPECT_EQ(get_frequencies(test_string4), frequencies4);
    EXPECT_EQ(get_frequencies(test_string5), frequencies5);
}

// Test cases
TEST(EncodeTest, SingleCharTest) {
    std::u32string test_string = U"a";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    EXPECT_EQ(encode(test_string, tree), U"0");
}

TEST(EncodeTest, AllSameCharactersTest) {
    std::u32string test_string = U"aaa";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    EXPECT_EQ(encode(test_string, tree), U"000");
}

TEST(EncodeTest, MixedWithSpaceTest) {
    std::u32string test_string = U"a b c d e f g h i j k l";
    // Ensure space is treated as a character

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));

    EXPECT_EQ(encode(test_string, tree), U"1000010010111100111000110110101101010011111011010011101011001011000");
}

TEST(EncodeTest, LongerStringTest) {
    std::u32string test_string = U"abcdefghijklmnopqrstuvwxyz ";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));

    // Expected encode for 26 letters + space
    EXPECT_EQ(encode(test_string, tree), U"1100110100001000111101000000001111001101110001101011100001000110110111101100111101110100001001101100111111110110010111100101101010");
}

TEST(TreeSerializationTest, SingleNodeTree) {
    Node* root = new Node{U'a', 1};
    EXPECT_EQ(serializeTree(root), U"1a");
    delete root;
}

TEST(TreeSerializationTest, SimpleTree) {
    Node* left = new Node{U'b', 1};
    Node* right = new Node{U'c', 1};
    Node* root = new Node{2, left, right};
    EXPECT_EQ(serializeTree(root), U"01b1c");
    delete left;
    delete right;
    delete root;
}

TEST(TreeSerializationTest, ComplexTree) {
    Node* left_left = new Node{U'd', 1};
    Node* left_right = new Node{U'e', 1};
    Node* right_left = new Node{U'f', 1};
    Node* right_right = new Node{U'g', 1};
    Node* left = new Node{2, left_left, left_right};
    Node* right = new Node{2, right_left, right_right};
    Node* root = new Node{4, left, right};
    EXPECT_EQ(serializeTree(root), U"001d1e01f1g");
    delete left_left;
    delete left_right;
    delete right_left;
    delete right_right;
    delete left;
    delete right;
    delete root;
}

TEST(TreeSerializationTest, EmptyTree) {
    EXPECT_EQ(serializeTree(nullptr), U"");
}

TEST(TreeDeserializationTest, SingleNodeTree) {
    std::u32string serialized = U"1a";
    HuffmanTree tree;
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    Node* child = root->left;
    EXPECT_EQ(root->right, nullptr);
    EXPECT_EQ(root->left, nullptr);
    EXPECT_EQ(root->symbol, U'a');
}

TEST(DataStripTest, SimpleTreeAndText) {
    // "00000002" => treeSize = 2
    // "1a"       => the tree (2 bytes)
    // "00000008" => bitCount = 8
    // "encodedtext" => remainder
    std::string raw_data = "000000021a00000008encodedtext";

    auto result = stripData(raw_data);
    EXPECT_EQ(std::get<0>(result), U"1a");          // The UTF-32 tree
    EXPECT_EQ(std::get<1>(result), 8);              // The bit count
    EXPECT_EQ(std::get<2>(result), "encodedtext");  // The remaining data
}

TEST(DataStripTest, EmptyTreeAndText) {
    // "00000000" => treeSize = 0, so no tree bytes
    // "00000000" => bitCount = 0
    // "encodedtext" => remainder
    std::string raw_data = "0000000000000000encodedtext";

    auto result = stripData(raw_data);
    EXPECT_EQ(std::get<0>(result), U"");            // Empty tree
    EXPECT_EQ(std::get<1>(result), 0);              // Bit count = 0
    EXPECT_EQ(std::get<2>(result), "encodedtext");  // The remaining data
}

TEST(DataStripTest, ComplexTreeAndText) {
    // "00000013" => treeSize = 13
    // "0010d1e010f1g" => the tree (13 bytes)
    // "00000012" => bitCount = 12
    // "encodedtext" => remainder
    std::string raw_data = "000000130010d1e010f1g00000012encodedtext";

    auto result = stripData(raw_data);
    EXPECT_EQ(std::get<0>(result), U"0010d1e010f1g");  // The UTF-32 tree
    EXPECT_EQ(std::get<1>(result), 12);               // The bit count
    EXPECT_EQ(std::get<2>(result), "encodedtext");    // The remaining data
}

TEST(DecodeTest, SingleCharTest) {
    std::u32string test_string = U"a";
    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::u32string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(DecodeTest, AllSameCharactersTest) {
    std::u32string test_string = U"aaa";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::u32string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(DecodeTest, MixedWithSpaceTest) {
    std::u32string test_string = U"a b c d e f g h i j k l";
    // Ensure space is treated as a character

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::u32string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(DecodeTest, LongerStringTest) {
    std::u32string test_string = U"abcdefghijklmnopqrstuvwxyz ";

    HuffmanTree tree;
    tree.build(get_frequencies(test_string));
    std::u32string encoded = encode(test_string, tree);
    EXPECT_EQ(decode(encoded, tree.getRoot()), test_string);
}

TEST(HuffmanEncodeTest, EmptyString) {
    std::u32string test_string = U"";
    testing::internal::CaptureStderr();

    std::string result = huffman_encode(test_string);

    std::string stderr_output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(result, "");
    EXPECT_EQ(stderr_output, "Error: Empty string\n");
}

TEST(HuffmanEncodeTest, SingleChar) {
    std::u32string test_string = U"a";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, AllSameCharacters) {
    std::u32string test_string = U"aaa";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, MixedWithSpace) {
    std::u32string test_string = U"a b c d e f g h i j k l";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, LongerString) {
    std::u32string test_string = U"abcdefghijklmnopqrstuvwxyz ";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanEncodeTest, ComplexString) {
    std::u32string test_string = U"The quick brown fox jumps over the lazy dog 1234567890!@#";
    std::string encoded = huffman_encode(test_string);
    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, EmptyString) {
    std::string bitstream = "";
    EXPECT_EQ(huffman_decode(bitstream), U"");
}

TEST(HuffmanDecodeTest, SingleChar) {
    std::u32string test_string = U"a";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, AllSameCharacters) {
    std::u32string test_string = U"aaa";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, MixedWithSpace) {
    std::u32string test_string = U"a b c d e f g h i j k l";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, LongerString) {
    std::u32string test_string = U"abcdefghijklmnopqrstuvwxyz ";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanDecodeTest, ComplexString) {
    std::u32string test_string = U"The quick brown fox jumps over the lazy dog 1234567890!@#";
    std::string encoded = huffman_encode(test_string);
    EXPECT_EQ(huffman_decode(encoded), test_string);
}

TEST(HuffmanTreeTest, GenerateCodesTest) {
    HuffmanTree tree;
    std::unordered_map<char32_t, int> frequencies = {{U'a', 5}, {U'b', 9}, {U'c', 12}, {U'd', 13}, {U'e', 16}, {U'f', 45}};
    tree.build(frequencies);
    std::unordered_map<char32_t, std::u32string> codes;
    codes = tree.getCodes();
    EXPECT_EQ(codes[U'a'], U"1100");
    EXPECT_EQ(codes[U'b'], U"1101");
    EXPECT_EQ(codes[U'c'], U"100");
    EXPECT_EQ(codes[U'd'], U"101");
    EXPECT_EQ(codes[U'e'], U"111");
    EXPECT_EQ(codes[U'f'], U"0");
}

TEST(HuffmanTreeTest, GenerateCodesTestWithSingleCharacter) {
    HuffmanTree tree;
    std::unordered_map<char32_t, int> frequencies = {{U'a', 1}};
    tree.build(frequencies);
    std::unordered_map<char32_t, std::u32string> codes;
    codes = tree.getCodes();
    EXPECT_EQ(codes[U'a'], U"0");
}

TEST(HuffmanTreeTest, GenerateCodesTestWithEqualFrequencies) {
    HuffmanTree tree;
    std::unordered_map<char32_t, int> frequencies = {{U'a', 1}, {U'b', 1}, {U'c', 1}, {U'd', 1}};
    tree.build(frequencies);
    std::unordered_map<char32_t, std::u32string> codes;
    codes = tree.getCodes();
    EXPECT_EQ(codes.size(), 4);
    EXPECT_TRUE(codes[U'a'].length() > 0);
    EXPECT_TRUE(codes[U'b'].length() > 0);
    EXPECT_TRUE(codes[U'c'].length() > 0);
    EXPECT_TRUE(codes[U'd'].length() > 0);
}

TEST(HuffmanTreeTest, BuildFromSerializedTreeTest) {
    HuffmanTree tree;
    std::u32string serialized = U"001d1e01f1g";
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->left->right->symbol, U'e');
    EXPECT_EQ(root->right->left->symbol, U'f');
    EXPECT_EQ(root->right->right->symbol, U'g');
}

TEST(HuffmanTreeTest, BuildFromSerializedTreeSingleNodeTest) {
    HuffmanTree tree;
    std::u32string serialized = U"1a";
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->symbol, U'a');
    EXPECT_EQ(root->left, nullptr);
    EXPECT_EQ(root->right, nullptr);
}

TEST(HuffmanTreeTest, BuildFromSerializedTreeComplexTest) {
    HuffmanTree tree;
    std::u32string serialized = U"001d1e01f01h1i";
    tree.rebuildTree(serialized);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->left->left->symbol, U'd');
    EXPECT_EQ(root->left->right->symbol, U'e');
    EXPECT_EQ(root->right->left->symbol, U'f');
    EXPECT_EQ(root->right->right->left->symbol, U'h');
    EXPECT_EQ(root->right->right->right->symbol, U'i');
}

TEST(HuffmanTreeTest, ConstructorTest) {
    HuffmanTree tree;
    EXPECT_EQ(tree.getRoot(), nullptr);
}

TEST(HuffmanTreeTest, ConstructorWithFrequenciesTest) {
    HuffmanTree tree;
    std::unordered_map<char32_t, int> frequencies = {{U'a', 1}, {U'b', 2}};
    tree.build(frequencies);
    EXPECT_NE(tree.getRoot(), nullptr);
}

TEST(HuffmanTreeTest, ConstructorWithSingleCharacterTest) {
    HuffmanTree tree;
    std::unordered_map<char32_t, int> frequencies = {{U'a', 1}};
    tree.build(frequencies);
    EXPECT_NE(tree.getRoot(), nullptr);
    EXPECT_EQ(tree.getRoot()->symbol, U'a');
}

TEST(HuffmanTreeTest, DestructorTest) {
    HuffmanTree* tree = new HuffmanTree();
    delete tree;
    // No explicit check here, just ensuring no crash on delete
}

TEST(HuffmanTreeTest, DestructorWithNodesTest) {
    HuffmanTree* tree = new HuffmanTree();
    std::unordered_map<char32_t, int> frequencies = {{U'a', 1}, {U'b', 2}};
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
    std::unordered_map<char32_t, int> frequencies = {{U'a', 1}};
    tree->build(frequencies);
    delete tree;
    // No explicit check here, just ensuring no crash on delete
}

TEST(HuffmanTreeTest, BuildTest) {
    HuffmanTree tree;
    std::unordered_map<char32_t, int> frequencies = {{U'a', 5}, {U'b', 9}, {U'c', 12}, {U'd', 13}, {U'e', 16}, {U'f', 45}};
    tree.build(frequencies);
    Node* root = tree.getRoot();
    EXPECT_EQ(root->frequency, 100);
    EXPECT_EQ(root->left->frequency, 45);
    EXPECT_EQ(root->right->frequency, 55);
}
