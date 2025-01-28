#ifndef LZ78_H
#define LZ78_H

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::tuple<int, char32_t>> LZ78_compress(const std::u32string& text);  // compress LZ78 algorithm
std::u32string LZ78_decompress(const std::unordered_map<std::u32string, int>& dictionary);  // decompress LZ78 algorithm
std::string LZ78_makebin(const std::vector<std::tuple<int, char32_t>> data);  // make binary from LZ78 compressed data
std::vector<std::tuple<int, char32_t> > readbin(std::string bitstream);


#endif  // LZ_78_H
