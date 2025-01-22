#include "utils.h"

// Converts std::u32string to std::string (UTF-8)
std::string u32ToUtf8(const std::u32string& u32str) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes(u32str);
}

std::u32string utf8ToU32(const std::string& utf8Str){
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.from_bytes(utf8Str);
}

std::string char32ToUtf8(char32_t c) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes(c);
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
