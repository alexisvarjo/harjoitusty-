#include <gtest/gtest.h>
#include <random>
#include <unordered_map>
#include <string>
#include <iostream>

#include "../src/LZ78.h"

// Helper function to generate random strings
std::string generate_random_string(size_t length) {
    // Basic ASCII letters, digits, and a few extended characters (all valid UTF-8)
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

// Test that encoding and then decoding returns the original input (random input)
TEST(LZ78Test, EncodeDecodeRandomInput) {
    for (int i = 0; i < 100; ++i) {
        std::string original_text = generate_random_string(100);
        std::string encoded_text = lz78_encode(original_text);
        std::string decoded_text = lz78_decode(encoded_text);
        EXPECT_EQ(original_text, decoded_text);
    }
}

// Test empty input
TEST(LZ78Test, EncodeDecodeEmptyString) {
    std::string input = "";
    std::string encoded_text = lz78_encode(input);
    std::string decoded_text = lz78_decode(encoded_text);
    EXPECT_EQ(input, decoded_text);
}

// Test single-character input
TEST(LZ78Test, EncodeDecodeSingleCharacter) {
    std::string input = "A";
    std::string encoded_text = lz78_encode(input);
    std::string decoded_text = lz78_decode(encoded_text);
    EXPECT_EQ(input, decoded_text);
}

// Test repeated-character input
TEST(LZ78Test, EncodeDecodeSameCharacter) {
    std::string input = "AAA";
    std::string encoded_text = lz78_encode(input);
    std::string decoded_text = lz78_decode(encoded_text);
    EXPECT_EQ(input, decoded_text);
}

// Unit test for LZ78_compress function
TEST(lz78_compress, Test1) {
    std::string input = "ABABABA";
    std::vector<std::tuple<int, char>> expected_output = {
        {0, 'A'}, {0, 'B'}, {1, 'B'}, {3, 'A'}, {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual_output = LZ78_compress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_compress for empty input
TEST(lz78_compress, EmptyInput) {
    std::string input = "";
    std::vector<std::tuple<int, char>> expected_output = {
        {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual_output = LZ78_compress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_compress for single-character input
TEST(lz78_compress, SingleCharacter) {
    std::string input = "A";
    std::vector<std::tuple<int, char>> expected_output = {
        {0, 'A'}, {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual_output = LZ78_compress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_compress for repeated characters
TEST(lz78_compress, RepeatedCharacters) {
    std::string input = "AAAAAA";
    std::vector<std::tuple<int, char>> expected_output = {
        {0, 'A'}, {1, 'A'}, {2, 'A'}, {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual_output = LZ78_compress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_compress for mixed input
TEST(lz78_compress, MixedInput) {
    std::string input = "ABCD";
    std::vector<std::tuple<int, char>> expected_output = {
        {0, 'A'}, {0, 'B'}, {0, 'C'}, {0, 'D'}, {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual_output = LZ78_compress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_compress for long input
TEST(lz78_compress, LongInput) {
    std::string input = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::vector<std::tuple<int, char>> expected_output = {
        // First A–Z: each letter is added as a new entry
        {0, 'A'}, {0, 'B'}, {0, 'C'}, {0, 'D'}, {0, 'E'},
        {0, 'F'}, {0, 'G'}, {0, 'H'}, {0, 'I'}, {0, 'J'},
        {0, 'K'}, {0, 'L'}, {0, 'M'}, {0, 'N'}, {0, 'O'},
        {0, 'P'}, {0, 'Q'}, {0, 'R'}, {0, 'S'}, {0, 'T'},
        {0, 'U'}, {0, 'V'}, {0, 'W'}, {0, 'X'}, {0, 'Y'}, {0, 'Z'},
        // Second A–Z: these entries are formed by concatenating previous entries
        {1, 'B'},   // "A" + "B" = "AB"
        {3, 'D'},   // "C" + "D" = "CD"
        {5, 'F'},   // "E" + "F" = "EF"
        {7, 'H'},   // "G" + "H" = "GH"
        {9, 'J'},   // "I" + "J" = "IJ"
        {11, 'L'},  // "K" + "L" = "KL"
        {13, 'N'},  // "M" + "N" = "MN"
        {15, 'P'},  // "O" + "P" = "OP"
        {17, 'R'},  // "Q" + "R" = "QR"
        {19, 'T'},  // "S" + "T" = "ST"
        {21, 'V'},  // "U" + "V" = "UV"
        {23, 'X'},  // "W" + "X" = "WX"
        {25, 'Z'},  // "Y" + "Z" = "YZ"
        // EOF marker
        {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual_output = LZ78_compress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Unit test for LZ78_decompress function
TEST(lz78_decompress, Test1) {
    std::vector<std::tuple<int, char>> input = {
        {0, 'A'}, {0, 'B'}, {1, 'B'}, {3, 'A'}, {-1, '\0'}
    };
    std::string expected_output = "ABABABA";
    std::string actual_output = LZ78_decompress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_decompress for empty input
TEST(lz78_decompress, EmptyInput) {
    std::vector<std::tuple<int, char>> input = { {-1, '\0'} };
    std::string expected_output = "";
    std::string actual_output = LZ78_decompress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_decompress for single-character input
TEST(lz78_decompress, SingleCharacter) {
    std::vector<std::tuple<int, char>> input = { {0, 'A'}, {-1, '\0'} };
    std::string expected_output = "A";
    std::string actual_output = LZ78_decompress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_decompress for repeated characters
TEST(lz78_decompress, RepeatedCharacters) {
    std::vector<std::tuple<int, char>> input = { {0, 'A'}, {1, 'A'}, {2, 'A'}, {-1, '\0'} };
    std::string expected_output = "AAAAAA";
    std::string actual_output = LZ78_decompress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test LZ78_decompress for mixed input
TEST(lz78_decompress, MixedInput) {
    std::vector<std::tuple<int, char>> input = { {0, 'A'}, {0, 'B'}, {0, 'C'}, {0, 'D'}, {-1, '\0'} };
    std::string expected_output = "ABCD";
    std::string actual_output = LZ78_decompress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// Test for makebin function (converting tuples to a compact binary string)
TEST(makebin, basecase) {
    std::vector<std::tuple<int, char>> compressedData = {
        {0, 'a'},
        {1, 'b'},
        {2, 'c'},
        {3, 'd'},
        {4, 'e'},
        {-1, '\0'} // EOF marker
    };

    static const unsigned char expectedBytes[] = {
        // (0, 'a')
        0x00,0x00,0x00,0x00,  0x61,
        // (1, 'b')
        0x01,0x00,0x00,0x00,  0x62,
        // (2, 'c')
        0x02,0x00,0x00,0x00,  0x63,
        // (3, 'd')
        0x03,0x00,0x00,0x00,  0x64,
        // (4, 'e')
        0x04,0x00,0x00,0x00,  0x65,
        // (-1, '\0')
        0xFF,0xFF,0xFF,0xFF,  0x00
    };

    std::string expected(reinterpret_cast<const char*>(expectedBytes), sizeof(expectedBytes));
    std::string actual = makebin(compressedData);
    EXPECT_EQ(expected, actual);
}

// Test for makebin with empty input
TEST(makebin, emptyInput) {
    std::vector<std::tuple<int, char>> compressedData = { {-1, '\0'} };

    static const unsigned char expectedBytes[] = {
        0xFF,0xFF,0xFF,0xFF,  0x00
    };

    std::string expected(reinterpret_cast<const char*>(expectedBytes), sizeof(expectedBytes));
    std::string actual = makebin(compressedData);
    EXPECT_EQ(expected, actual);
}

// Test for makebin with a single character
TEST(makebin, singleCharacter) {
    std::vector<std::tuple<int, char>> compressedData = {
        {0, 'a'},
        {-1, '\0'}
    };

    static const unsigned char expectedBytes[] = {
        0x00,0x00,0x00,0x00,  0x61,
        0xFF,0xFF,0xFF,0xFF,  0x00,
    };

    std::string expected(reinterpret_cast<const char*>(expectedBytes), sizeof(expectedBytes));
    std::string actual = makebin(compressedData);
    EXPECT_EQ(expected, actual);
}

// Test for readbin function (reading a binary string into tuples)
TEST(readbin, basecase) {
    static const unsigned char bitstreamBytes[] = {
        0x00,0x00,0x00,0x00,  0x61,  // (0, 'a')
        0x01,0x00,0x00,0x00,  0x62,  // (1, 'b')
        0x02,0x00,0x00,0x00,  0x63,  // (2, 'c')
        0x03,0x00,0x00,0x00,  0x64,  // (3, 'd')
        0x04,0x00,0x00,0x00,  0x65,  // (4, 'e')
        0xFF,0xFF,0xFF,0xFF,  0x00,   // (-1, '\0')
    };
    std::string bitstream(reinterpret_cast<const char*>(bitstreamBytes), sizeof(bitstreamBytes));
    std::vector<std::tuple<int, char>> expected = {
        {0, 'a'},
        {1, 'b'},
        {2, 'c'},
        {3, 'd'},
        {4, 'e'},
        {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual = readbin(bitstream);
    EXPECT_EQ(expected, actual);
}

// Test for readbin with empty input
TEST(readbin, emptyInput) {
    static const unsigned char bitstreamBytes[] = {
        0xFF,0xFF,0xFF,0xFF,  0x00,
    };
    std::string bitstream(reinterpret_cast<const char*>(bitstreamBytes), sizeof(bitstreamBytes));
    std::vector<std::tuple<int, char>> expected = {
        {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual = readbin(bitstream);
    EXPECT_EQ(expected, actual);
}

// Test for readbin with a single character
TEST(readbin, singleCharacter) {
    static const unsigned char bitstreamBytes[] = {
        0x00,0x00,0x00,0x00,  0x61,  // (0, 'a')
        0xFF,0xFF,0xFF,0xFF,  0x00,
    };
    std::string bitstream(reinterpret_cast<const char*>(bitstreamBytes), sizeof(bitstreamBytes));
    std::vector<std::tuple<int, char>> expected = {
        {0, 'a'},
        {-1, '\0'}
    };
    std::vector<std::tuple<int, char>> actual = readbin(bitstream);
    EXPECT_EQ(expected, actual);
}
