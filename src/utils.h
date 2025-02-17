#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>


// tämä otsaketiedosto ja utils.cpp sisältävät apufunktioita,
// joita tarvitaan molemmissa algoritmeissa

// kirjoittaa sisällön annettuun tiedostoon binäärinä
int writefile(const std::filesystem::path& path, const std::string& content);

// lukee binääritiedostosta sisällön ja palauttaa sen merkkijonona
std::string readfile(const std::filesystem::path& fp);

bool areFilesIdentical(const std::string& file1, const std::string& file2);

std::string unpackBits(const std::string &packed, size_t bitCount);

std::string packBits(const std::string &bits);

std::vector<std::filesystem::path> getFiles(std::filesystem::path fp);

#endif  // SRC_UTILS_H_
