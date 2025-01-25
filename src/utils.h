#ifndef utils_h
#define utils_h

#include <codecvt>
#include <locale>
#include <string>

#include <unicode/unistr.h>

std::string u32ToUtf8(const std::u32string& u32str); // std::u32string -> std::string (UTF-8)
std::u32string charToU32String(const char* c); // char -> std::u32string
std::string char32ToUtf8(char32_t c);
std::u32string utf8ToU32(const std::string& utf8Str); // std::string (UTF-8) -> std::u32string
std::string sizeTtoUtf8(size_t s); // size_t -> std::u32string

#endif
