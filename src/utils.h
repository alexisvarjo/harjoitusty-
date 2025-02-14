#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <unicode/unistr.h>

#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>


// tämä otsaketiedosto ja utils.cpp sisältävät apufunktioita,
// joita tarvitaan molemmissa algoritmeissa

// tietotyyppimuunnos std::u32string -> std::string (UTF-8)
std::string u32ToUtf8(const std::u32string& u32str);

// tietotyyppimuunnos char -> std::u32string
std::u32string charToU32String(const char* c);

// tietotyyppimuunnos char32_t -> std::string (UTF-8)
std::string char32ToUtf8(char32_t c);

// tietotyyppimuunnos std::string (UTF-8) -> std::u32string
std::u32string utf8ToU32(const std::string& utf8Str);

// tietotyyppimuunnos size_t -> std::u32string
std::string sizeTtoUtf8(size_t s);

// kirjoittaa sisällön annettuun tiedostoon binäärinä
void writefile(const std::filesystem::path& path, const std::string& content);

// lukee binääritiedostosta sisällön ja palauttaa sen merkkijonona
std::string readfile(const std::filesystem::path& fp);

bool areFilesIdentical(const std::string& file1, const std::string& file2);

std::u32string unpackBits(const std::string &packed, size_t bitCount);

std::string packBits(const std::u32string &bits);

#endif  // SRC_UTILS_H_
