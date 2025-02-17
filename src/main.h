#ifndef MAIN_H
#define MAIN_H

#include "huffman.h"
#include "LZ78.h"
#include "utils.h"
#include "main.h"
#include <filesystem>
#include <iostream>

int test_functions(std::vector<std::filesystem::path> files_vector);
std::vector<std::filesystem::path> getFiles(std::filesystem::path fp);
int main(int argc, char *argv[]);
#endif //MAIN_H
