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
