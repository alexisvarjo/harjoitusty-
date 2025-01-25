#include "utils.h"

std::string u32ToUtf8(const std::u32string& u32str) {
    icu::UnicodeString unicode_str;
    for (char32_t c : u32str){
        unicode_str.append(static_cast<UChar32>(c));
    }
    std::string utf8_text;
    unicode_str.toUTF8String(utf8_text);
    return utf8_text;
}

std::u32string utf8ToU32(const std::string& utf8Str){
    icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(utf8Str);
    std::u32string utf32_text;
    for (int32_t i = 0; i < unicode_str.length();){
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

std::string sizeTtoUtf8(size_t s){
    return std::to_string(s);
}

std::u32string charToU32String(const char* c) {
    if (c == nullptr || c[0] == '\0' ){
        return std::u32string();
    }
    std::u32string u32str;
    while (*c){
        u32str += static_cast<char32_t>(*c);
        ++c;
    }

    return u32str;
}
