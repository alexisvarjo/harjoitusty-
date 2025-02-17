#include <gtest/gtest.h>

#include "../src/utils.h"


TEST(UtilsTest, WriteToFileTestBasic) {
    std::string filename = "test_files/testfile.txt";
    std::string content = "test";
    writefile(filename, content);
    std::ifstream rf(filename);
    std::string read_content;
    rf >> read_content;
    rf.close();
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, WriteToFileTestEmpty) {
    std::filesystem::path filename = "test_files/testfile_empty.txt";
    std::string content = "";
    writefile(filename, content);
    std::ifstream rf(filename);
    std::string read_content;
    rf >> read_content;
    rf.close();
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, WriteToFileTestSpecialCharacters) {
    std::string filename = "test_files/testfile_special.txt";
    std::string content = "tëstî";
    writefile(filename, content);
    std::ifstream rf(filename);
    std::string read_content;
    rf >> read_content;
    rf.close();
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfiletest) {
    std::string filename = "test_files/testfile.txt";
    std::string content = "test";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfiletest_empty) {
    std::string filename = "test_files/testfile_empty.txt";
    std::string content = "";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfiletest_special_characters) {
    std::string filename = "test_files/testfile_special.txt";
    std::string content = "tëstî";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfiletest_long_string) {
    std::string filename = "test_files/testfile_long.txt";
    std::string content = "this is a longer test string with multiple characters";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfiletest_overwrite) {
    std::string filename = "test_files/testfile_overwrite.txt";
    std::string content1 = "first content";
    std::string content2 = "second content";
    writefile(filename, content1);
    writefile(filename, content2);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content2, read_content);
}

TEST(UtilsTest, readfiletest_binary_data) {
    std::string filename = "test_files/testfile_binary.txt";
    std::string content = "\x00\x01\x02\x03\x04\x05";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfile_basic) {
    std::string filename = "test_files/testfile.txt";
    std::string content = "test";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfile_empty) {
    std::string filename = "test_files/testfile_empty.txt";
    std::string content = "";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfile_special_characters) {
    std::string filename = "test_files/testfile_special.txt";
    std::string content = "tëstî";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfile_long_string) {
    std::string filename = "test_files/testfile_long.txt";
    std::string content = "this is a longer test string with multiple characters";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, readfile_overwrite) {
    std::string filename = "test_files/testfile_overwrite.txt";
    std::string content1 = "first content";
    std::string content2 = "second content";
    writefile(filename, content1);
    writefile(filename, content2);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content2, read_content);
}

TEST(UtilsTest, readfile_binary_data) {
    std::string filename = "test_files/testfile_binary.txt";
    std::string content = "\x00\x01\x02\x03\x04\x05";
    writefile(filename, content);
    std::string read_content = readfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, AreFilesIdentical_SameContent) {
    std::string fileA = "test_files/test_areFilesIdentical_1a.txt";
    std::string fileB = "test_files/test_areFilesIdentical_1b.txt";
    std::string content = "same content";
    writefile(fileA, content);
    writefile(fileB, content);
    EXPECT_TRUE(areFilesIdentical(fileA, fileB));
}

TEST(UtilsTest, AreFilesIdentical_DifferentContent) {
    std::string fileA = "test_files/test_areFilesIdentical_2a.txt";
    std::string fileB = "test_files/test_areFilesIdentical_2b.txt";
    writefile(fileA, "abc");
    writefile(fileB, "xyz");
    EXPECT_FALSE(areFilesIdentical(fileA, fileB));
}

TEST(UtilsTest, PackBits_Basic) {
    std::string bits = "10101010";
    std::string packed = packBits(bits);
    EXPECT_EQ(static_cast<size_t>(1), packed.size());
    EXPECT_EQ('\xAA', packed[0]);
}

TEST(UtilsTest, PackBits_Partial) {
    std::string bits = "110";
    std::string packed = packBits(bits);
    EXPECT_EQ(static_cast<size_t>(1), packed.size());
    // 110 followed by five zeros -> 11000000 in binary -> 0xC0
    EXPECT_EQ('\xC0', packed[0]);
}

TEST(UtilsTest, PackBits_MultipleBytes) {
    std::string bits = "11011100000";
    std::string packed = packBits(bits);
    EXPECT_EQ(static_cast<size_t>(2), packed.size());
    EXPECT_EQ('\xDC', packed[0]);  // 0xDC = 220
    EXPECT_EQ('\x00', packed[1]);  // 0x00 = 0
}

TEST(UtilsTest, PackBits_MultipleBytesPartial) {
    std::string bits = "1101110000011";
    std::string packed = packBits(bits);
    EXPECT_EQ(static_cast<size_t>(2), packed.size());
    EXPECT_EQ('\xDC', packed[0]);  // same first 8 bits as above
    EXPECT_EQ('\x18', packed[1]);  // 0x18 = 24
}

TEST(UtilsTest, PackBits_MultipleBytesPartial2) {
    std::string bits = "11011100000111";
    std::string packed = packBits(bits);
    EXPECT_EQ(static_cast<size_t>(2), packed.size());
    EXPECT_EQ('\xDC', packed[0]);  // again, first 8 bits are "11011100"
    EXPECT_EQ('\x1C', packed[1]);  // 0x1C = 28
}

TEST(UtilsTest, UnpackBits_Basic) {
    std::string packed = "\xAA"; // 10101010
    size_t bitCount = 8;
    std::string bits = unpackBits(packed, bitCount);
    EXPECT_EQ("10101010", bits);
}

TEST(UtilsTest, UnpackBits_PartialByte) {
    std::string packed = "\xC0"; // 11000000
    size_t bitCount = 3;
    std::string bits = unpackBits(packed, bitCount);
    EXPECT_EQ("110", bits);
}

TEST(UtilsTest, UnpackBits_MultipleBytes) {
    // 0xDC = 11011100, 0x00 = 00000000
    std::string packed = "\xDC\x00";
    size_t bitCount = 10;
    std::string bits = unpackBits(packed, bitCount);
    // Reads 8 bits from 0xDC and 2 bits from 0x00.
    EXPECT_EQ("11011100", bits);
}

TEST(UtilsTest, UnpackBits_MultipleBytesPartial) {
    // 0xDC = 11011100, 0x00 = 00000000
    std::string packed = "\xDC\x00";
    size_t bitCount = 11;
    std::string bits = unpackBits(packed, bitCount);
    // Reads 8 bits from 0xDC and 3 bits from 0x00.
    EXPECT_EQ("11011100", bits);
}

TEST(UtilsTest, getFilesTest_NonExistentPath) {
    std::filesystem::path path("test_files/non_existent_path");
    std::vector<std::filesystem::path> result = getFiles(path);
    EXPECT_TRUE(result.empty());
}

TEST(UtilsTest, getFilesTest_SingleFile) {
    std::filesystem::path path("test_files/single_file_test.txt");
    std::string content = "Single file content";
    writefile(path, content);

    std::vector<std::filesystem::path> result = getFiles(path);
    EXPECT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].filename().string(), "single_file_test.txt");
}

TEST(UtilsTest, getFilesTest_DirectoryWithMultipleFiles) {
    std::filesystem::path dirPath("test_files/multiple_files_test");
    std::filesystem::create_directories(dirPath);

    std::filesystem::path file1 = dirPath / "file1.txt";
    std::filesystem::path file2 = dirPath / "file2.txt";
    std::filesystem::path file3 = dirPath / "file3.txt";

    writefile(file1, "File1 content");
    writefile(file2, "File2 content");
    writefile(file3, "File3 content");

    std::vector<std::filesystem::path> result = getFiles(dirPath);
    EXPECT_EQ(result.size(), 3u);

    // Sort the filenames for a deterministic comparison
    std::vector<std::string> filenames;
    filenames.reserve(result.size());
    for (const auto &p : result) {
        filenames.push_back(p.filename().string());
    }
    std::sort(filenames.begin(), filenames.end());

    EXPECT_EQ(filenames[0], "file1.txt");
    EXPECT_EQ(filenames[1], "file2.txt");
    EXPECT_EQ(filenames[2], "file3.txt");
}
