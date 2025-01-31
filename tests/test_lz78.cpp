#include <gtest/gtest.h>
#include "../src/LZ78.h"

// Apufunktio satunnaissyötteille
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

// Testi onko syöte sama purkamisen ja pakkaamisen jälkeen
TEST(LZ78Test, EncodeDecodeRandomInput) {
    for (int i = 0; i < 100; ++i) {
        std::u32string original_text = generate_random_u32string(100);
        std::string encoded_text = lz78_encode(original_text);
        std::u32string decoded_text = lz78_decode(encoded_text);
        EXPECT_EQ(original_text, decoded_text);
    }
}

// testi tyhjälle syötteelle
TEST(LZ78Test, EncodeDecodeEmptyString) {
    std::u32string input = U"";
    std::string encoded_text = lz78_encode(input);
    std::u32string decoded_text = lz78_decode(encoded_text);
    EXPECT_EQ(input, decoded_text);
}

// testi yhdelle merkille
TEST(LZ78Test, EncodeDecodeSingleCharacter) {
    std::u32string input = U"A";
    std::string encoded_text = lz78_encode(input);
    std::u32string decoded_text = lz78_decode(encoded_text);
    EXPECT_EQ(input, decoded_text);
}

// yksikkötesti lz78_compress-funktiolle
TEST(lz78_compress, Test1) {
    std::u32string input = U"ABABABA";
    std::vector<std::tuple<int, char32_t> > expected_output = {{0, U'A'}, {0, U'B'}, {1, U'B'}, {3, U'A'}, {-1, U'\0'}};
    std::vector<std::tuple<int, char32_t> > actual_output = LZ78_compress(input);
    EXPECT_EQ(expected_output, actual_output);
}

// yksikkötesti lz78_decompress-funktiolle
TEST(lz78_decompress, Test1) {
    std::vector<std::tuple<int, char32_t> > input = {{0, U'A'}, {0, U'B'}, {1, U'B'}, {3, U'A'}, {-1, U'\0'}};
    std::u32string expected_output = U"ABABABA";
    std::u32string actual_output = LZ78_decompress(input);
    EXPECT_EQ(expected_output, actual_output);
}


// testi makebin-funktiolle
TEST(makebin, basecase) {
    // Input for makebin
    std::vector<std::tuple<int, char32_t>> compressedData {
        {0, U'a'},
        {1, U'b'},
        {2, U'c'},
        {3, U'd'},
        {4, U'e'},
        {-1, U'\0'} // EOF marker
    };

    static const unsigned char expectedBytes[] = {
        // (0, 'a') => index=0, char='a'
        0x00,0x00,0x00,0x00,  0x61,0x00,0x00,0x00,
        // (1, 'b')
        0x01,0x00,0x00,0x00,  0x62,0x00,0x00,0x00,
        // (2, 'c')
        0x02,0x00,0x00,0x00,  0x63,0x00,0x00,0x00,
        // (3, 'd')
        0x03,0x00,0x00,0x00,  0x64,0x00,0x00,0x00,
        // (4, 'e')
        0x04,0x00,0x00,0x00,  0x65,0x00,0x00,0x00,
        // (-1, '\0')
        0xFF,0xFF,0xFF,0xFF,  0x00,0x00,0x00,0x00
    };

    // std::string näistä
    std::string expected(
        reinterpret_cast<const char*>(expectedBytes),
        sizeof(expectedBytes)
    );

    std::string actual = makebin(compressedData);

    EXPECT_EQ(expected, actual);
}


// readbin-yksikkötesti
TEST(readbin, basecase) {
    static const unsigned char bitstreamBytes[] = {
        0x00,0x00,0x00,0x00,  0x61,0x00,0x00,0x00,  // (0, 'a')
        0x01,0x00,0x00,0x00,  0x62,0x00,0x00,0x00,  // (1, 'b')
        0x02,0x00,0x00,0x00,  0x63,0x00,0x00,0x00,  // (2, 'c')
        0x03,0x00,0x00,0x00,  0x64,0x00,0x00,0x00,  // (3, 'd')
        0x04,0x00,0x00,0x00,  0x65,0x00,0x00,0x00,  // (4, 'e')
        0xFF,0xFF,0xFF,0xFF,  0x00,0x00,0x00,0x00   // (-1, '\0')
    };
    std::string bitstream(
        reinterpret_cast<const char*>(bitstreamBytes),
        sizeof(bitstreamBytes)
    );

    std::vector<std::tuple<int, char32_t>> expected {
        {0,  U'a'},
        {1,  U'b'},
        {2,  U'c'},
        {3,  U'd'},
        {4,  U'e'},
        {-1, U'\0'}
    };
    std::vector<std::tuple<int, char32_t>> actual = readbin(bitstream);
    EXPECT_EQ(expected, actual);
}
