#ifndef MAIN_H
#define MAIN_H

#include "huffman.h"
#include "LZ78.h"
#include "utils.h"
#include "main.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sys/signal.h>

int test_functions(std::vector<std::filesystem::path> files_vector);
int main(int argc, char *argv[]);
#endif //MAIN_H
