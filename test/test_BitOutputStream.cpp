#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    bos.writeBit(1);
    bos.writeBit(0);
    bos.flush();

    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST(BitOutputStreamTests, LONG_TEST) {
    stringstream ss;
    BitOutputStream bos(ss);
    for (int i = 0; i < 15; i++) {
        bos.writeBit((i + 1) % 2);
    }
    bos.flush();

    string bitsStr = "10101010";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
    ASSERT_EQ(ss.get(), asciiVal);
}
