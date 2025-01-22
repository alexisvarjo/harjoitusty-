#ifndef utils_h
#define utils_h

#include <string>
#include <codecvt>
#include <locale>

std::string u32ToUtf8(const std::u32string& u32str); // std::u32string -> std::string (UTF-8)
std::u32string charToU32String(const char* c); // char -> std::u32string
std::string char32ToUtf8(char32_t c);
std::u32string utf8ToU32(const std::string& utf8Str); // std::string (UTF-8) -> std::u32string


#endif
