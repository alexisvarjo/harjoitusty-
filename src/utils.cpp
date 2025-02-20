#include "utils.h"

//luetaan tiedosto. funktio päättelee tiedostopäätteestä lukumoodin
std::string readfile(const std::filesystem::path& fp) {
    std::ifstream rf;
    std::string ext = fp.extension().string();
    std::string content;

    if (ext == ".txt") {
        rf.open(fp, std::ios::in);
        if (!rf) {
            std::cerr << "Error opening file " << fp.string() << std::endl;
            return "";
        }
        content = std::string((std::istreambuf_iterator<char>(rf)), std::istreambuf_iterator<char>());
        // rf will be closed automatically when it goes out of scope.
    } else if (ext == ".bin") {
        rf.open(fp, std::ios::in | std::ios::binary);
        if (!rf) {
            std::cerr << "Error opening file " << fp.string() << std::endl;
            return "";
        }
    } else {
        std::cerr << "Unsupported file type: " << ext << std::endl;
        return "";
    }
    const std::size_t chunkSize = 256 * 1024; // 256kB chunk
    std::vector<char> buffer(chunkSize);

    while (rf){
        rf.read(buffer.data(), buffer.size());
        std::streamsize bytesRead = rf.gcount();
        if (bytesRead > 0){
            content.append(buffer.data(), static_cast<size_t>(bytesRead));
        }
    }
    return content;
}

//kirjoitetaan tiedosto. funktio päättelee tiedostopäätteestä kirjoitustyypin
int writefile(const std::filesystem::path& path, const std::string& content) {
    std::ofstream wf(path, std::ios::out | std::ios::binary);
    if (!wf) {
        std::cerr << "Error opening file " << path.string() << std::endl;
        return 1;
    }

    const std::size_t chunkSize = 256 * 1024; // 256kB chunk
    size_t pos = 0;

    while (pos < content.size()){
        size_t bytesToWrite = std::min(chunkSize, content.size() - pos);
        wf.write(content.data() + pos, bytesToWrite);
        if (!wf){
            std::cerr << "Error writing to file " << path.string() << std::endl;
            return 1;
        }
        pos += bytesToWrite;
    }
    wf.close();
    return 0;
}

// tarkistetaan ovatko tiedostot bittitasolla samat.
bool areFilesIdentical(const std::filesystem::path& file1, const std::filesystem::path& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1 || !f2) {
        std::cerr << "Error opening files " << file1 << " and " << file2 << std::endl;
        return false;
    }

    int c1, c2;
    while (true) {
        c1 = f1.get();
        c2 = f2.get();

        if (c1 == EOF && c2 == EOF)
            break;

        if (c1 != c2)
            return false;
    }

    return true;
}


// Packs a string of bits (each character is '0' or '1') into a compact binary string.
std::string packBits(const std::string &bits) {
    std::string out;
    out.reserve((bits.size()+7)/8);
    uint8_t current = 0;
    int count = 0;
    for (char bit : bits) {
        current = (current << 1) | (bit == '1' ? 1 : 0);
        count++;
        if (count == 8) {
            out.push_back(static_cast<char>(current));
            current = 0;
            count = 0;
        }
    }
    if (count > 0) {
        current = current << (8 - count);
        out.push_back(static_cast<char>(current));
    }
    return out;
}

// puretaan bitit
std::string unpackBits(const std::string &packed, size_t bitCount) {
    std::string bits;
    for (size_t i = 0; i < packed.size() && bitCount > 0; ++i) {
        uint8_t byte = static_cast<uint8_t>(packed[i]);
        int bitsToRead = (bitCount >= 8) ? 8 : static_cast<int>(bitCount);
        for (int j = 7; j >= 8 - bitsToRead; --j) {
            bits.push_back((byte & (1 << j)) ? '1' : '0');
        }
        bitCount = (bitCount >= 8) ? bitCount - 8 : 0;
    }
    return bits;
}

// mainissa tällä haetaan kaikki kansion sisältämät tiedostot
std::vector<std::filesystem::path> getFiles(std::filesystem::path fp) {
    std::vector<std::filesystem::path> files_vector;
    if (std::filesystem::exists(fp)){
        if (std::filesystem::is_directory(fp)) {
            for (const auto &entry : std::filesystem::directory_iterator(fp)) {
                files_vector.push_back(entry.path());
            }
        }
        else {
            files_vector.push_back(fp);
        }
    } else {
        std::cout << "File not found" << std::endl;
        std::vector<std::filesystem::path> empty;
        return empty;
    }
    return files_vector;
}
