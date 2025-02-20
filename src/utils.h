#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <vector>


// tämä otsaketiedosto ja utils.cpp sisältävät apufunktioita,
// joita tarvitaan molemmissa algoritmeissa

// kirjoittaa sisällön annettuun tiedostoon binäärinä
int writefile(const std::filesystem::path& path, const std::string& content);

// lukee binääritiedostosta sisällön ja palauttaa sen merkkijonona
std::string readfile(const std::filesystem::path& fp);

// tiedostojen yhdenmukaisuuden vertailu test_functions
bool areFilesIdentical(const std::filesystem::path& file1, const std::filesystem::path& file2);

// bittien purkaminen
std::string unpackBits(const std::string &packed, size_t bitCount);

// bittien pakkaaminen
std::string packBits(const std::string &bits);

// haetaan kaikki kansion tiedostot
std::vector<std::filesystem::path> getFiles(std::filesystem::path fp);

#endif  // SRC_UTILS_H_
