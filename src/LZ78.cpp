#include "LZ78.h"
#include "utils.h"

std::vector<std::tuple<int, char> > LZ78_compress(const std::string& text) {
    std::unordered_map<std::string, int> dictionary;
    std::vector<std::tuple<int, char>> compressed;
    int index = 1;
    std::string sequence = "";
    for (size_t i = 0; i < text.size(); ++i) {
        sequence.push_back(text[i]);

        if (dictionary.find(sequence) == dictionary.end()) {
            dictionary[sequence] = index++;

            std::string prefix = sequence.substr(0, sequence.size()-1);
            char newChar = sequence.back();

            int prefixIndex = 0;

            if (!prefix.empty()) {
                prefixIndex = dictionary[prefix];
            }
            compressed.emplace_back(prefixIndex, newChar);
            sequence.clear();
        }
    }
    if (!sequence.empty()) {
        std::string prefix = sequence.substr(0, sequence.size()-1);
        char newChar = sequence.back();

        int prefixIndex = 0;
        if (!prefix.empty()) {
            prefixIndex = dictionary[prefix];
        }
        compressed.emplace_back(prefixIndex, newChar);
    }

    compressed.push_back(std::make_tuple(-1, '\0')); // end of file -merkintä

    return compressed;
}

std::string makebin(const std::vector<std::tuple<int, char> >& compressedData) {
    std::ostringstream oss(std::ios::binary);

    if (compressedData.empty()) {
        int prefixIndex = -1;
        char newChar = '\0';
        oss.write(reinterpret_cast<const char*>(&prefixIndex), sizeof(prefixIndex));
        oss.write(reinterpret_cast<const char*>(&newChar), sizeof(newChar));
        return oss.str();
    }

    for (std::vector<std::tuple<int, char> >::const_iterator it = compressedData.begin(); it != compressedData.end(); ++it) {
        int prefixIndex = std::get<0>(*it);
        char newChar = std::get<1>(*it);

        oss.write(reinterpret_cast<const char*>(&prefixIndex), sizeof(prefixIndex));
        oss.write(reinterpret_cast<const char*>(&newChar), sizeof(newChar));
    }
    return oss.str();
}

std::vector<std::tuple<int, char>> readbin(std::string& bitstream) {
    std::vector<std::tuple<int, char>> compressed;
    std::istringstream iss(bitstream, std::ios::binary);
    while (iss) {
        int prefixIndex;
        char newChar;
        if (iss.read(reinterpret_cast<char*>(&prefixIndex), sizeof(prefixIndex)) && iss.read(reinterpret_cast<char*>(&newChar), sizeof(newChar))) {
            compressed.emplace_back(prefixIndex, newChar);
        } else if(iss.eof()){
            break;
        } else{
            throw std::runtime_error("Error reading binary file");
        }
    }
    return compressed;
}


std::string LZ78_decompress(const std::vector<std::tuple<int, char>>& dictionary) {
    std::string decompressed = "";
    std::unordered_map<int, std::string> dict;
    int index = 1;
    for (size_t i = 0; i < dictionary.size(); ++i) {
        int prefixIndex = std::get<0>(dictionary[i]);
        char32_t newChar = std::get<1>(dictionary[i]);
        // end of file -merkinnän tarkistus
        if (prefixIndex == -1 && newChar == U'\0') {
            break;
        }
        std::string sequence;
        if (prefixIndex > 0) {
            sequence = dict[prefixIndex];
        }
        sequence += newChar;
        dict[index++] = sequence;
        decompressed += sequence;
    }
    return decompressed;
}

std::string lz78_encode(const std::string& text) {
    std::vector<std::tuple<int, char>> compressed = LZ78_compress(text);
    std::string bin = makebin(compressed);
    return bin;
}

std::string lz78_decode(const std::string& bitstream) {
    std::vector<std::tuple<int, char>> compressed = readbin(bitstream);
    std::string decompressed = LZ78_decompress(compressed);
    return decompressed;
}
