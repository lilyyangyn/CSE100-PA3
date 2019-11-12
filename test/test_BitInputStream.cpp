#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitInputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitInputStreamTests, SIMPLE_TEST) {
    string bitsStr = "10000000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);

    ASSERT_EQ(1, bis.readBit());
    ASSERT_EQ(0, bis.readBit());
}

TEST(BitInputStreamTests, LONG_TEST) {
    string bitsStr = "10101010";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii + ascii);
    BitInputStream bis(ss);

    for (int i = 0; i < 15; i++) {
        EXPECT_EQ((i + 1) % 2, bis.readBit());
    }
}
