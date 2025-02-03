#include "utils.h"

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
