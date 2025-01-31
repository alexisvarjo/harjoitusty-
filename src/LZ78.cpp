#include "LZ78.h"
#include "utils.h"

std::vector<std::tuple<int, char32_t> > LZ78_compress(const std::u32string& text) {
    std::unordered_map<std::u32string, int> dictionary;
    std::vector<std::tuple<int, char32_t>> compressed;
    int index = 1;
    std::u32string sequence = charToU32String("");
    for (size_t i = 0; i < text.size(); ++i) {
        sequence.push_back(text[i]);

        if (dictionary.find(sequence) == dictionary.end()) {
            dictionary[sequence] = index++;

            std::u32string prefix = sequence.substr(0, sequence.size()-1);
            char32_t newChar = sequence.back();

            int prefixIndex = 0;

            if (!prefix.empty()) {
                prefixIndex = dictionary[prefix];
            }
            compressed.emplace_back(prefixIndex, newChar);
            sequence.clear();
        }
    }
    if (!sequence.empty()) {
        std::u32string prefix = sequence.substr(0, sequence.size()-1);
        char32_t newChar = sequence.back();

        int prefixIndex = 0;
        if (!prefix.empty()) {
            prefixIndex = dictionary[prefix];
        }
        compressed.emplace_back(prefixIndex, newChar);
    }

    compressed.push_back(std::make_tuple(-1, U'\0')); // end of file -merkintä

    return compressed;
}

std::string makebin(const std::vector<std::tuple<int, char32_t> > compressedData) {
    std::ostringstream oss(std::ios::binary);

    if (compressedData.empty()) {
        int prefixIndex = -1;
        char32_t newChar = U'\0';
        oss.write(reinterpret_cast<const char*>(&prefixIndex), sizeof(prefixIndex));
        oss.write(reinterpret_cast<const char*>(&newChar), sizeof(newChar));
        return oss.str();
    }

    for (std::vector<std::tuple<int, char32_t> >::const_iterator it = compressedData.begin(); it != compressedData.end(); ++it) {
        int prefixIndex = std::get<0>(*it);
        char32_t newChar = std::get<1>(*it);

        oss.write(reinterpret_cast<const char*>(&prefixIndex), sizeof(prefixIndex));
        oss.write(reinterpret_cast<const char*>(&newChar), sizeof(newChar));
    }
    return oss.str();
}

std::vector<std::tuple<int, char32_t> > readbin(std::string bitstream) {
    std::vector<std::tuple<int, char32_t>> compressed;
    std::istringstream iss(bitstream, std::ios::binary);
    while (iss) {
        int prefixIndex;
        char32_t newChar;
        iss.read(reinterpret_cast<char*>(&prefixIndex), sizeof(prefixIndex));
        iss.read(reinterpret_cast<char*>(&newChar), sizeof(newChar));
        compressed.emplace_back(prefixIndex, newChar);
    }
    compressed.pop_back(); // poistetaan lopetusmerkintä
    return compressed;
}


std::u32string LZ78_decompress(const std::vector<std::tuple<int, char32_t> >& dictionary) {
    std::u32string decompressed = charToU32String("");
    std::unordered_map<int, std::u32string> dict;
    int index = 1;
    for (size_t i = 0; i < dictionary.size(); ++i) {
        // end of file -merkinnän tarkistus
        if (std::get<0>(dictionary[i]) == -1 && std::get<1>(dictionary[i]) == U'\0') {
            break;
        }
        int prefixIndex = std::get<0>(dictionary[i]);
        char32_t newChar = std::get<1>(dictionary[i]);
        std::u32string prefix = charToU32String("");
        if (prefixIndex != 0) {
            prefix = dict[prefixIndex];
        }
        std::u32string sequence = prefix + newChar;
        dict[index++] = sequence;
        decompressed += sequence;
    }
    return decompressed;
}

std::string lz78_encode(const std::u32string& text) {
    std::vector<std::tuple<int, char32_t>> compressed = LZ78_compress(text);
    std::string bin = makebin(compressed);
    return bin;
}

std::u32string lz78_decode(const std::string& bitstream) {
    std::vector<std::tuple<int, char32_t>> compressed = readbin(bitstream);
    std::u32string decompressed = LZ78_decompress(compressed);
    return decompressed;
}
