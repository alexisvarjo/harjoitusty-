#include "main.h"

int test_functions(std::string filename) {
    std::string content;
    std::vector<std::string> files;
    if (std::filesystem::exists(filename)) {
        if (std::filesystem::is_directory(filename)) {
            for (const auto &entry : std::filesystem::directory_iterator(filename)) {
                files.push_back(entry.path().string());
            }
        }
        else {
            files.push_back(filename);
        }
    } else {
        std::cout << "File not found" << std::endl;
        return 1;
    }
    for (const std::string &file : files) {
        std::filesystem::path filePath(file);
        std::string ext = filePath.extension().string();
        if (ext == ".txt") {
            content = read_textfile(file);
        } else if (ext == ".bin") {
            content = read_from_file(file);
        } else {
            return 1;
        }
        std::u32string u32content = utf8ToU32(content);
        auto encode_start_huffman = std::chrono::high_resolution_clock::now();
        std::string huffman_encoded = huffman_encode(u32content);
        auto encode_end_huffman = std::chrono::high_resolution_clock::now();
        auto encode_start_lz78 = std::chrono::high_resolution_clock::now();
        std::string lz78_encoded = lz78_encode(u32content);
        auto encode_end_lz78 = std::chrono::high_resolution_clock::now();

        std::filesystem::path dir = filePath.parent_path();
        std::string stem = filePath.stem().string();
        std::filesystem::path huffEncPath = dir / (stem + "_huffman_encoded.bin");
        std::filesystem::path lzEncPath = dir / (stem + "_lz78_encoded.bin");

        write_to_file(huffEncPath.string(), huffman_encoded);
        write_to_file(lzEncPath.string(), lz78_encoded);

        auto size_huffman = std::filesystem::file_size(huffEncPath.string());
        auto size_lz78 = std::filesystem::file_size(lzEncPath.string());
        auto size_original = std::filesystem::file_size(file);

        std::string huffman_encoded_fromfile = read_from_file(huffEncPath.string());
        std::string lz78_encoded_fromfile = read_from_file(lzEncPath.string());

        auto decode_start_huffman = std::chrono::high_resolution_clock::now();
        std::u32string huffman_decoded = huffman_decode(huffman_encoded_fromfile);
        auto decode_end_huffman = std::chrono::high_resolution_clock::now();
        auto decode_start_lz78 = std::chrono::high_resolution_clock::now();
        std::u32string lz78_decoded = lz78_decode(lz78_encoded_fromfile);
        auto decode_end_lz78 = std::chrono::high_resolution_clock::now();
        std::string huffman_decoded_utf8 = u32ToUtf8(huffman_decoded);
        std::string lz78_decoded_utf8 = u32ToUtf8(lz78_decoded);

        std::filesystem::path huffDecPath = dir / (stem + "_huffman_decoded.txt");
        std::filesystem::path lzDecPath = dir / (stem + "_lz78_decoded.txt");

        write_as_textfile(huffDecPath.string(), huffman_decoded_utf8);
        write_as_textfile(lzDecPath.string(), lz78_decoded_utf8);

        if (!areFilesIdentical(file, huffDecPath.string())) {
            std::cout << "Huffman decoding failed" << std::endl;
            return 1;
        }
        if (!areFilesIdentical(file, lzDecPath.string())) {
            std::cout << "LZ78 decoding failed" << std::endl;
            return 1;
        }
        std::chrono::duration<double, std::milli> huffman_encode_duration = encode_end_huffman - encode_start_huffman;
        std::chrono::duration<double, std::milli> lz78_encode_duration = encode_end_lz78 - encode_start_lz78;
        std::chrono::duration<double, std::milli> huffman_decode_duration = decode_end_huffman - decode_start_huffman;
        std::chrono::duration<double, std::milli> lz78_decode_duration = decode_end_lz78 - decode_start_lz78;

        double compress_ratio_huffman = (double)size_huffman / size_original;
        double compress_ratio_lz78 = (double)size_lz78 / size_original;

        std::cout << "Huffman Encoding Time: " << huffman_encode_duration.count() << " ms";
        std::cout << " vs. LZ78 Encoding Time: " << lz78_encode_duration.count() << " ms" << std::endl;

        std::cout << "Huffman Decoding Time: " << huffman_decode_duration.count() << " ms";
        std::cout << " vs. LZ78 Decoding Time: " << lz78_decode_duration.count() << " ms" << std::endl;

        std::cout << "Huffman Encoded File Size: " << size_huffman << " bytes";
        std::cout << " vs. LZ78 Encoded File Size: " << size_lz78 << " bytes" << std::endl;

        std::cout << "Huffman Compression ratio: " << compress_ratio_huffman;
        std::cout << " vs. LZ78 Compression ratio: " << compress_ratio_lz78 << std::endl;

        std::cout << "Both algorithms returned same file as they were given." << std::endl;

        std::filesystem::remove(huffEncPath.string());
        std::filesystem::remove(lzEncPath.string());
        std::filesystem::remove(huffDecPath.string());
        std::filesystem::remove(lzDecPath.string());
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./main <filename> <algorithm, 'h' or 'lz'> <e/d> <p>" << std::endl;
        return 1;
    }
    if (argc > 4) {
        std::cout << "Too many arguments" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::filesystem::path filePath(filename);

    // When doing encoding or decoding, use the same directory for the output file.
    if (argc == 4) {
        std::string algo = argv[2];
        if (algo != "h" && algo != "lz") {
            std::cout << "Invalid argument" << std::endl;
            return 1;
        }
        std::string encodingmode = argv[3];
        if (encodingmode != "e" && encodingmode != "d") {
            std::cout << "Invalid argument" << std::endl;
            return 1;
        }
        if (encodingmode == "e") {
            // Encoding: read the text file and output as .bin, then remove the original .txt file.
            std::string content = read_textfile(filename);
            std::u32string u32content = utf8ToU32(content);
            std::filesystem::path outPath = filePath;
            outPath.replace_extension(".bin");
            if (algo == "h") {
                std::string encoded = huffman_encode(u32content);
                write_to_file(outPath.string(), encoded);
            } else if (algo == "lz") {
                std::string encoded = lz78_encode(u32content);
                write_to_file(outPath.string(), encoded);
            }
            std::filesystem::remove(filename);
        } else if (encodingmode == "d") {
            // Decoding: read the .bin file and output as .txt, then remove the .bin file.
            std::string encoded = read_from_file(filename);
            std::filesystem::path outPath = filePath;
            outPath.replace_extension(".txt");
            if (algo == "h") {
                std::u32string decoded = huffman_decode(encoded);
                std::string decoded_utf8 = u32ToUtf8(decoded);
                write_to_file(outPath.string(), decoded_utf8);
            } else if (algo == "lz") {
                std::u32string decoded = lz78_decode(encoded);
                std::string decoded_utf8 = u32ToUtf8(decoded);
                write_to_file(outPath.string(), decoded_utf8);
            }
            std::filesystem::remove(filename);
        }
        return 0;
    }
    if (argc == 3) {
        std::string test = argv[2];
        if (test != "p") {
            std::cout << "Invalid argument" << std::endl;
            return 1;
        }
        return test_functions(filename);
    }
    return 0;
}
