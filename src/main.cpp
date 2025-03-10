#include "main.h"


//tällä sievennetään main funktion ulkoasua
int test_functions(std::vector<std::filesystem::path> files_vector) {
    for (const std::filesystem::path &file : files_vector) {
        std::string content = readfile(file);
        if (content.empty())
            continue;
        std::cout << "Testing with file: " << file.string() << std::endl;
        std::filesystem::path dir = file.parent_path();
        std::string stem = file.stem().string();
        std::filesystem::path huffEncPath = dir / (stem + "_huffman_encoded.bin");
        std::filesystem::path lzEncPath = dir / (stem + "_lz78_encoded.bin");
        std::filesystem::path huffDecPath = dir / (stem + "_huffman_decoded.txt");
        std::filesystem::path lzDecPath = dir / (stem + "_lz78_decoded.txt");
        std::chrono::time_point<std::chrono::high_resolution_clock> encode_start_huffman;
        std::chrono::time_point<std::chrono::high_resolution_clock> encode_end_huffman;
        std::chrono::time_point<std::chrono::high_resolution_clock> encode_start_lz78;
        std::chrono::time_point<std::chrono::high_resolution_clock> encode_end_lz78;
        std::chrono::time_point<std::chrono::high_resolution_clock> decode_start_huffman;
        std::chrono::time_point<std::chrono::high_resolution_clock> decode_end_huffman;
        std::chrono::time_point<std::chrono::high_resolution_clock> decode_start_lz78;
        std::chrono::time_point<std::chrono::high_resolution_clock> decode_end_lz78;
    {
        encode_start_huffman = std::chrono::high_resolution_clock::now();
        std::string huffman_encoded = huffman_encode(content);
        encode_end_huffman = std::chrono::high_resolution_clock::now();
        if (writefile(huffEncPath, huffman_encoded) == 1){
            std::cout << "Error whilst writing Huffman encoded file"<< huffEncPath.string() << std::endl;
            return 1;
        }
    }
    {
        encode_start_lz78 = std::chrono::high_resolution_clock::now();
        std::string lz78_encoded = lz78_encode(content);
        encode_end_lz78 = std::chrono::high_resolution_clock::now();
        if (writefile(lzEncPath, lz78_encoded) == 1){
            std::cout << "Error whilst writing LZ encoded file"<< lzEncPath.string() << std::endl;
            return 1;
        }
    }
        auto size_huffman = std::filesystem::file_size(huffEncPath);
        auto size_lz78 = std::filesystem::file_size(lzEncPath);
        auto size_original = std::filesystem::file_size(file);
    {
        std::string huffman_encoded_fromfile = readfile(huffEncPath);
        decode_start_huffman = std::chrono::high_resolution_clock::now();
        std::string huffman_decoded = huffman_decode(huffman_encoded_fromfile);
        decode_end_huffman = std::chrono::high_resolution_clock::now();
        if (writefile(huffDecPath, huffman_decoded) == 1) {
            std::cout << "Error whilst writing huffman decoded file " << huffDecPath.string() << std::endl;
            return 1;
        }
    }
    {
        std::string lz78_encoded_fromfile = readfile(lzEncPath);
        decode_start_lz78 = std::chrono::high_resolution_clock::now();
        std::string lz78_decoded = lz78_decode(lz78_encoded_fromfile);
        decode_end_lz78 = std::chrono::high_resolution_clock::now();
        if (writefile(lzDecPath, lz78_decoded) == 1) {
            std::cout << "Error whilst writing lz78 decoded file " << lzDecPath.string() << std::endl;
            return 1;
        }
    }
        if (!areFilesIdentical(file, huffDecPath)) {
            std::cout << "Huffman decoding failed as the input and output file weren't identical" << std::endl;
            return 1;
        }
        if (!areFilesIdentical(file, lzDecPath)) {
            std::cout << "LZ78 decoding failed as the input and output file weren't identical" << std::endl;
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

        std::filesystem::remove(huffEncPath);
        std::filesystem::remove(lzEncPath);
        std::filesystem::remove(huffDecPath);
        std::filesystem::remove(lzDecPath);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    try{
    if (argc < 2) {
        std::cout << "Usage: ./main <filename> <algorithm/mode, 'h' or 'lz', 'p'>" << std::endl;
        return 1;
    }
    if (argc > 3) {
        std::cout << "Too many arguments" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::filesystem::path filePath(filename);
    if (!std::filesystem::exists(filePath)) {
        std::cout << "File does not exist" << std::endl;
        return 1;
    }
    std::vector<std::filesystem::path> files_vector = getFiles(filePath);
    bool encodingmode;
    if (filePath.extension() == ".txt") {
        encodingmode = true;
    } else if (filePath.extension() == ".bin") {
        encodingmode = false;
    } else if (std::filesystem::is_directory(filePath)){
        if (files_vector.empty()) {
            std::cout << "Directory is empty" << std::endl;
            return 1;
        } else {
            if (files_vector[0].extension() == ".txt") {
                encodingmode = true;
            } else if (files_vector[0].extension() == ".bin") {
                encodingmode = false;
            } else {
                std::cout << "Unsupported file type in directory" << std::endl;
                std::cout << files_vector[0].string() << std::endl;
                return 1;
            }
        }
    }
    std::string algo;
    // When doing encoding or decoding, use the same directory for the output file.
    if (argc == 3 || (argc == 2 && encodingmode == false)) {
        if (argc == 3){
            algo = argv[2];
        }
        if (algo == "h" || algo == "lz" || encodingmode == false){
            for (std::filesystem::path& fp : files_vector) {
                std::string filename = fp.string();
                std::cout << "Processing file: " << filename << std::endl;
                if (encodingmode == true) {
                    // Encoding mode: process only .txt files.
                    if (fp.extension() != ".txt"){
                        std::cout << "Skipping non-text file: " << filename << std::endl;
                        continue;
                    }
                    std::string content = readfile(fp);
                    if (content.empty())
                        continue;
                    std::filesystem::path outPath = fp;
                    outPath.replace_extension(".bin");
                    if (algo == "h") {
                        std::string encoded = huffman_encode(content);
                        std::string marker = "0"; //merkitään Huffman pakkaamisen merkiksi purkamista varten
                        encoded = marker + encoded;
                        if (writefile(outPath, encoded) == 1){
                            std::cout << "Error whilst writing file " << outPath.string() << std::endl;
                            return 1;
                        }
                    } else if (algo == "lz") {
                        std::string encoded = lz78_encode(content);
                        std::string marker = "1";  //merkitään lz78 pakkaamisen merkiksi purkamista varten
                        encoded = marker + encoded;
                        if (writefile(outPath, encoded) == 1){
                            std::cout << "Error whilst writing file " << outPath.string() << std::endl;
                            return 1;
                        }
                    }
                    std::filesystem::remove(fp);
                } else {
                    // Decoding mode: process only .bin files.
                    if (fp.extension() != ".bin"){
                        std::cout << "Skipping non-binary file: " << filename << std::endl;
                        continue;
                    }
                    std::string encoded = readfile(fp);
                    if (encoded.empty())
                        continue;
                    std::filesystem::path outPath = fp;
                    outPath.replace_extension(".txt");
                    if (encoded[0] == '0') { // aikaisemmin asetettu merkintä
                        encoded = encoded.substr(1);
                        std::string decoded = huffman_decode(encoded);
                        if (writefile(outPath, decoded) == 1){
                            std::cout << "Error whilst writing file " << outPath.string() << std::endl;
                            return 1;
                        }
                    } else if (encoded[0] == '1') { // aikaisemmin asetettu merkintä
                        encoded = encoded.substr(1);
                        std::string decoded = lz78_decode(encoded);
                        if (writefile(outPath, decoded) == 1){
                            std::cout << "Error whilst writing file " << outPath.string() << std::endl;
                            return 1;
                        }
                    }
                    std::filesystem::remove(filename);
                }
                std::cout << "Processed file: " << filename << std::endl;
            }
            return 0;
        } else if (algo == "p"){
            return test_functions(files_vector);
        }
    }
    return 1;
    }
    catch(std::exception& e){
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
}
