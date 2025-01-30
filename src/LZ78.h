#ifndef SRC_LZ78_H_
#define SRC_LZ78_H_

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>

// compress LZ78 algorithm

std::vector<std::tuple<int, char32_t>> LZ78_compress(const std::u32string& text);

// decompress LZ78 algorithm
std::u32string LZ78_decompress(const std::unordered_map<std::u32string, int>& dictionary);

// make binary from LZ78 compressed data
std::string LZ78_makebin(const std::vector<std::tuple<int, char32_t>> data);
// read binary data to LZ78 compressed data
std::vector<std::tuple<int, char32_t> > readbin(std::string bitstream);

#endif  // SRC_LZ78_H_
