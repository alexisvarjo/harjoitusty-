#include <gtest/gtest.h>


#include "../src/utils.h"

TEST(UtilsTest, U32ToUtf8TestBasic) {
    std::u32string u32str = U"testi";
    std::string utf8 = u32ToUtf8(u32str);
    EXPECT_EQ(utf8, "testi");
}

TEST(UtilsTest, U32ToUtf8TestEmpty) {
    std::u32string u32str = U"";
    std::string utf8 = u32ToUtf8(u32str);
    EXPECT_EQ(utf8, "");
}

TEST(UtilsTest, U32ToUtf8TestSpecialCharacters) {
    std::u32string u32str = U"tëstî";
    std::string utf8 = u32ToUtf8(u32str);
    EXPECT_EQ(utf8, "tëstî");
}

TEST(UtilsTest, U32ToUtf8TestEmoji) {
    std::u32string u32str = U"😀";
    std::string utf8 = u32ToUtf8(u32str);
    EXPECT_EQ(utf8, "😀");
}

TEST(UtilsTest, U32ToUtf8TestLongString) {
    std::u32string u32str = U"this is a longer test string with multiple characters";
    std::string utf8 = u32ToUtf8(u32str);
    EXPECT_EQ(utf8, "this is a longer test string with multiple characters");
}

TEST(UtilsTest, Utf8ToU32TestBasic) {
    std::string utf8 = "testi";
    std::u32string u32str = utf8ToU32(utf8);
    EXPECT_EQ(u32str, U"testi");
}

TEST(UtilsTest, Utf8ToU32TestEmpty) {
    std::string utf8 = "";
    std::u32string u32str = utf8ToU32(utf8);
    EXPECT_EQ(u32str, U"");
}

TEST(UtilsTest, Utf8ToU32TestSpecialCharacters) {
    std::string utf8 = "tëstî";
    std::u32string u32str = utf8ToU32(utf8);
    EXPECT_EQ(u32str, U"tëstî");
}

TEST(UtilsTest, Utf8ToU32TestEmoji) {
    std::string utf8 = "😀";
    std::u32string u32str = utf8ToU32(utf8);
    EXPECT_EQ(u32str, U"😀");
}

TEST(UtilsTest, Utf8ToU32TestLongString) {
    std::string utf8 = "this is a longer test string with multiple characters";
    std::u32string u32str = utf8ToU32(utf8);
    EXPECT_EQ(u32str, U"this is a longer test string with multiple characters");
}

TEST(UtilsTest, Char32ToUtf8TestBasic) {
    char32_t c = U'a';
    std::string utf8 = char32ToUtf8(c);
    EXPECT_EQ(utf8, "a");
}

TEST(UtilsTest, Char32ToUtf8TestSpecialCharacter) {
    char32_t c = U'ñ';
    std::string utf8 = char32ToUtf8(c);
    EXPECT_EQ(utf8, "ñ");
}

TEST(UtilsTest, Char32ToUtf8TestEmoji) {
    char32_t c = U'😀';
    std::string utf8 = char32ToUtf8(c);
    EXPECT_EQ(utf8, "😀");
}

TEST(UtilsTest, Char32ToUtf8TestNonPrintableCharacter) {
    char32_t c = U'\u0001';
    std::string utf8 = char32ToUtf8(c);
    EXPECT_EQ(utf8, "\u0001");
}

TEST(UtilsTest, SizeTtoUtf8TestBasic) {
    size_t s = 123;
    std::string utf8 = sizeTtoUtf8(s);
    EXPECT_EQ(utf8, "123");
}

TEST(UtilsTest, SizeTtoUtf8TestZero) {
    size_t s = 0;
    std::string utf8 = sizeTtoUtf8(s);
    EXPECT_EQ(utf8, "0");
}

TEST(UtilsTest, SizeTtoUtf8TestLargeNumber) {
    size_t s = 123456789;
    std::string utf8 = sizeTtoUtf8(s);
    EXPECT_EQ(utf8, "123456789");
}

TEST(UtilsTest, SizeTtoUtf8TestMaxSizeT) {
    size_t s = std::numeric_limits<size_t>::max();
    std::string utf8 = sizeTtoUtf8(s);
    EXPECT_EQ(utf8, std::to_string(std::numeric_limits<size_t>::max()));
}

TEST(UtilsTest, SizeTtoUtf8TestSingleDigit) {
    size_t s = 7;
    std::string utf8 = sizeTtoUtf8(s);
    EXPECT_EQ(utf8, "7");
}

TEST(UtilsTest, CharToU32StringTest) {
    const char* c = "testi";
    std::u32string u32str = charToU32String(c);
    EXPECT_EQ(u32str, U"testi");
}

TEST(UtilsTest, CharToU32StringTestEmpty) {
    const char* c = "";
    std::u32string u32str = charToU32String(c);
    EXPECT_EQ(u32str, U"");
}

TEST(UtilsTest, CharToU32StringTestLongString) {
    const char* c = "this is a longer test string with multiple characters";
    std::u32string u32str = charToU32String(c);
    EXPECT_EQ(u32str, U"this is a longer test string with multiple characters");
}

TEST(UtilsTest, WriteToFileTestBasic) {
    std::string filename = "testfile.txt";
    std::string content = "test";
    write_to_file(filename, content);
    std::ifstream rf(filename);
    std::string read_content;
    rf >> read_content;
    rf.close();
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, WriteToFileTestEmpty) {
    std::string filename = "testfile_empty.txt";
    std::string content = "";
    write_to_file(filename, content);
    std::ifstream rf(filename);
    std::string read_content;
    rf >> read_content;
    rf.close();
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, WriteToFileTestSpecialCharacters) {
    std::string filename = "testfile_special.txt";
    std::string content = "tëstî";
    write_to_file(filename, content);
    std::ifstream rf(filename);
    std::string read_content;
    rf >> read_content;
    rf.close();
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_from_filetest) {
    std::string filename = "testfile.txt";
    std::string content = "test";
    write_to_file(filename, content);
    std::string read_content = read_from_file(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_from_filetest_empty) {
    std::string filename = "testfile_empty.txt";
    std::string content = "";
    write_to_file(filename, content);
    std::string read_content = read_from_file(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_from_filetest_special_characters) {
    std::string filename = "testfile_special.txt";
    std::string content = "tëstî";
    write_to_file(filename, content);
    std::string read_content = read_from_file(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_from_filetest_long_string) {
    std::string filename = "testfile_long.txt";
    std::string content = "this is a longer test string with multiple characters";
    write_to_file(filename, content);
    std::string read_content = read_from_file(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_from_filetest_overwrite) {
    std::string filename = "testfile_overwrite.txt";
    std::string content1 = "first content";
    std::string content2 = "second content";
    write_to_file(filename, content1);
    write_to_file(filename, content2);
    std::string read_content = read_from_file(filename);
    EXPECT_EQ(content2, read_content);
}

TEST(UtilsTest, read_from_filetest_binary_data) {
    std::string filename = "testfile_binary.txt";
    std::string content = "\x00\x01\x02\x03\x04\x05";
    write_to_file(filename, content);
    std::string read_content = read_from_file(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_textfile_basic) {
    std::string filename = "testfile.txt";
    std::string content = "test";
    write_to_file(filename, content);
    std::string read_content = read_textfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_textfile_empty) {
    std::string filename = "testfile_empty.txt";
    std::string content = "";
    write_to_file(filename, content);
    std::string read_content = read_textfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_textfile_special_characters) {
    std::string filename = "testfile_special.txt";
    std::string content = "tëstî";
    write_to_file(filename, content);
    std::string read_content = read_textfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_textfile_long_string) {
    std::string filename = "testfile_long.txt";
    std::string content = "this is a longer test string with multiple characters";
    write_to_file(filename, content);
    std::string read_content = read_textfile(filename);
    EXPECT_EQ(content, read_content);
}

TEST(UtilsTest, read_textfile_overwrite) {
    std::string filename = "testfile_overwrite.txt";
    std::string content1 = "first content";
    std::string content2 = "second content";
    write_to_file(filename, content1);
    write_to_file(filename, content2);
    std::string read_content = read_textfile(filename);
    EXPECT_EQ(content2, read_content);
}

TEST(UtilsTest, read_textfile_binary_data) {
    std::string filename = "testfile_binary.txt";
    std::string content = "\x00\x01\x02\x03\x04\x05";
    write_to_file(filename, content);
    std::string read_content = read_textfile(filename);
    EXPECT_EQ(content, read_content);
}
