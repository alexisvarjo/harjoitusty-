#include "../src/huffman.h"

#include <algorithm>
#include <random>

#include <gtest/gtest.h>

#include "../src/utils.h"


//apufunktio satunnaisten u32stringien generoimiseen
std::u32string generate_random_u32string(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0x0000, 0x10FFFF);

    std::u32string random_string;
    for (size_t i = 0; i < length; ++i) {
        uint32_t random_char = dis(gen);

        // jättää pois tietyt ongelmalliset luvut (0xD800 - 0xDFFF)
        if (random_char >= 0xD800 && random_char <= 0xDFFF) {
            --i;
            continue;
        }
        random_string.push_back(static_cast<char32_t>(random_char));
    }
    return random_string;
}


//testi, jossa testataan onko syötetty teksti sama kuin pakkauksen jälkeen purettu teksti
TEST(RandomInputTest, FunctionReturnsCorrectOutput) {
    const size_t test_count = 10000; //kuinka monta syötettä
    const size_t max_length = 100; //maksimipituus

    for (size_t i = 0; i < test_count; ++i) {
        std::u32string random_string = generate_random_u32string(max_length);
        EXPECT_EQ(huffman_decode(huffman_encode(random_string)), random_string);
    }
}
