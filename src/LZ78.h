#ifndef SRC_LZ78_H_
#define SRC_LZ78_H_

#include <iostream>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>

// compress LZ78 algorithm
std::vector<std::tuple<int, char>> LZ78_compress(const std::string& text);

// decompress LZ78 algorithm
std::string LZ78_decompress(const std::vector<std::tuple<int, char> >& dictionary);

// make binary from LZ78 compressed data
std::string makebin(const std::vector<std::tuple<int, char> >& compressedData);

// read binary data to LZ78 compressed data
std::vector<std::tuple<int, char>> readbin(std::string& bitstream);

std::string lz78_decode(const std::string& bitstream);

std::string lz78_encode(const std::string& text);



#endif  // SRC_LZ78_H_
