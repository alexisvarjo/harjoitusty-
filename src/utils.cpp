#include "utils.h"
#include <fstream>

std::string u32ToUtf8(const std::u32string& u32str) {
    icu::UnicodeString unicode_str;
    for (char32_t c : u32str) {
        unicode_str.append(static_cast<UChar32>(c));
    }
    std::string utf8_text;
    unicode_str.toUTF8String(utf8_text);
    return utf8_text;
}

std::u32string utf8ToU32(const std::string& utf8Str) {
    icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(utf8Str);
    std::u32string utf32_text;
    for (int32_t i = 0; i < unicode_str.length();) {
        UChar32 c = unicode_str.char32At(i);
        utf32_text.push_back(c);
        i += U16_LENGTH(c);
    }
    return utf32_text;
}

std::string char32ToUtf8(char32_t c) {
    icu::UnicodeString unicode_str;
    unicode_str.append(static_cast<UChar32>(c));

    std::string utf8;
    unicode_str.toUTF8String(utf8);

    return utf8;
}

std::string sizeTtoUtf8(size_t s) {
    return std::to_string(s);
}

std::u32string charToU32String(const char* c) {
    if (c == nullptr || c[0] == '\0') {
        return std::u32string();
    }
    std::u32string u32str;
    while (*c) {
        u32str += static_cast<char32_t>(*c);
        ++c;
    }

    return u32str;
}

void write_to_file(const std::string& filename, const std::string& content) {
    std::ofstream wf(filename, std::ios::out | std::ios::binary);
    if (!wf) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }
    wf.write(content.c_str(), content.size());
    wf.close();
}

std::string read_from_file(const std::string& filename) {
    std::ifstream rf(filename, std::ios::in | std::ios::binary);
    if (!rf) {
        std::cerr << "Error opening file " << filename << std::endl;
        return "";
    }
    rf.seekg(0, std::ios::end);
    std::streampos length = rf.tellg();
    rf.seekg(0, std::ios::beg);
    std::string content(length, '\0');
    rf.read(&content[0], length);
    if (rf.is_open()) {
        rf.close();
    }
    return content;
}

std::string read_textfile(const std::string& filename) {
    std::ifstream rf(filename, std::ios::in);
    rf.imbue(std::locale(""));
    if (!rf) {
        std::cerr << "Error opening file " << filename << std::endl;
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(rf)), std::istreambuf_iterator<char>());
    rf.close();
    return content;
}

bool areFilesIdentical(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1 || !f2) {
        std::cerr << "Error opening files " << file1 << " and " << file2 << std::endl;
        return false;
    }

    int c1, c2;
    while (true) {
        c1 = f1.get();
        c2 = f2.get();

        if (c1 == EOF && c2 == EOF)
            break;

        if (c1 != c2)
            return false;
    }

    return true;
}

void write_as_textfile(const std::string& filename, const std::string& content) {
    std::ofstream wf(filename);
    wf.imbue(std::locale(""));
    if (!wf) {
        std::cerr << "Error opening file " << filename << std::endl;
        return;
    }
    wf << content;
    wf.close();
}

std::vector<bool> concat_v(const std::vector<bool>& v1, const std::vector<bool>& v2) {
    std::vector<bool> v3;
    v3.reserve(v1.size() + v2.size());
    v3.insert(v3.end(), v1.begin(), v1.end());
    v3.insert(v3.end(), v2.begin(), v2.end());
    return v3;
}

// Packs a u32string of bits (each character is '0' or '1') into a compact binary string.
std::string packBits(const std::u32string &bits) {
    std::string out;
    uint8_t current = 0;
    int count = 0;
    for (char32_t bit : bits) {
        current = (current << 1) | (bit == U'1' ? 1 : 0);
        count++;
        if (count == 8) {
            out.push_back(static_cast<char>(current));
            current = 0;
            count = 0;
        }
    }
    if (count > 0) {
        current = current << (8 - count);
        out.push_back(static_cast<char>(current));
    }
    return out;
}

std::u32string unpackBits(const std::string &packed, size_t bitCount) {
    std::u32string bits;
    for (size_t i = 0; i < packed.size() && bitCount > 0; ++i) {
        uint8_t byte = static_cast<uint8_t>(packed[i]);
        int bitsToRead = (bitCount >= 8) ? 8 : static_cast<int>(bitCount);
        for (int j = 7; j >= 8 - bitsToRead; --j) {
            bits.push_back((byte & (1 << j)) ? U'1' : U'0');
        }
        bitCount = (bitCount >= 8) ? bitCount - 8 : 0;
    }
    return bits;
}
