#ifndef utils_h
#define utils_h

#include <codecvt>
#include <locale>
#include <string>

#include <unicode/unistr.h>

//tämä otsaketiedosto ja utils.cpp sisältävät apufunktioita, joita tarvitaan molemmissa algoritmeissa

std::string u32ToUtf8(const std::u32string& u32str); // tietotyyppimuunnos std::u32string -> std::string (UTF-8)
std::u32string charToU32String(const char* c); // tietotyyppimuunnos char -> std::u32string
std::string char32ToUtf8(char32_t c); //tietotyyppimuunnos char32_t -> std::string (UTF-8)
std::u32string utf8ToU32(const std::string& utf8Str); // tietotyyppimuunnos std::string (UTF-8) -> std::u32string
std::string sizeTtoUtf8(size_t s); // tietotyyppimuunnos size_t -> std::u32string

#endif
