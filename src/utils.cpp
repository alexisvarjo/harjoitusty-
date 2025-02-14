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

std::string readfile(const std::filesystem::path& fp) {
    std::string ext = fp.extension().string();
    std::string content;
    if (ext == ".txt") {
        std::ifstream rf(fp, std::ios::in);
        if (!rf) {
            std::cerr << "Error opening file " << fp.string() << std::endl;
            return "";
        }
        content = std::string((std::istreambuf_iterator<char>(rf)), std::istreambuf_iterator<char>());
        // rf will be closed automatically when it goes out of scope.
    } else if (ext == ".bin") {
        std::ifstream rf(fp, std::ios::in | std::ios::binary);
        if (!rf) {
            std::cerr << "Error opening file " << fp.string() << std::endl;
            return "";
        }
        rf.seekg(0, std::ios::end);
        std::streampos length = rf.tellg();
        rf.seekg(0, std::ios::beg);
        content.resize(length);
        rf.read(&content[0], length);
        // rf will be closed automatically when it goes out of scope.
    } else {
        std::cerr << "Unsupported file type: " << ext << std::endl;
        return "";
    }
    return content;
}

void writefile(const std::filesystem::path& path, const std::string& content) {
    if (path.extension() == ".txt") {
        std::ofstream wf(path, std::ios::out | std::ios::binary);
        if (!wf) {
            std::cerr << "Error opening file " << path.string() << std::endl;
            return;
        }
        wf.write(content.c_str(), content.size());
    wf.close();
    } else if (path.extension() == ".bin") {
        std::ofstream wf(path, std::ios::out | std::ios::binary);
        if (!wf) {
            std::cerr << "Error opening file " << path.string() << std::endl;
            return;
        }
        wf.write(content.c_str(), content.size());
        wf.close();
    }
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
