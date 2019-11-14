/**
 * This file performs unit tests for HCTree.
 *
 * Author: Yuening YANG, Shenlang Zhou
 * Email: y3yang@ucse.edu
 */
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree2.hpp"

using namespace std;
using namespace testing;

class SimpleHCTree2Fixture : public ::testing::Test {
  protected:
    HCTree2 tree;

  public:
    SimpleHCTree2Fixture() {
        // initialization code here
        vector<unsigned int> freqs(65536);
        unsigned char first, second;
        byte2 node;
        first = 'a';
        second = 'b';
        node = first << 8 + second;
        freqs[node] = 2;
        first = 'c';
        second = 'd';
        node = first << 8 + second;
        freqs[node] = 3;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTree2Fixture, TEST_ENCODE_BIT) {
    // test normal encoding
    vector<unsigned int> freqs(65536);
    unsigned char first, second;
    byte2 node;
    first = 'a';
    second = 'b';
    node = first << 8 + second;

    stringstream ss;
    BitOutputStream bos(ss);
    tree.encode(node, bos);
    bos.flush();
    string bitsStr = "00000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST_F(SimpleHCTree2Fixture, TEST_DECODE_BIT) {
    // test normal encoding
    vector<unsigned int> freqs(65536);
    unsigned char first, second;
    byte2 node;
    first = 'a';
    second = 'b';
    node = first << 8 + second;

    string bitsStr = "00000000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);
    ASSERT_EQ(tree.decode(bis), node);
}

TEST_F(SimpleHCTree2Fixture, TEST_GETDISTINCTCHAR) {
    EXPECT_EQ(tree.getDistinctChars(), 2);
}

TEST_F(SimpleHCTree2Fixture, TEST_GETTREE) {
    stringstream ss;
    BitOutputStream bos(ss);

    string bitsStr = "11000010";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    tree.getTree(bos);
    EXPECT_EQ(ss.get(), asciiVal);
}

/* Empty Tree & One-Node Tree Tests */
TEST(HCTree2Tests, SMALL_TEST_ENCODE_BIT) {
    HCTree2 tree1, tree2;
    vector<unsigned int> freqs(65536);

    stringstream ss;
    BitOutputStream bos1(ss);
    string bitsStr = "00000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    // empty tree
    tree1.build(freqs);
    tree1.encode(0, bos1);
    bos1.flush();
    EXPECT_EQ(ss.get(), asciiVal);

    ss.str("");
    BitOutputStream bos2(ss);
    // one-node tree
    unsigned char first, second;
    byte2 node;
    first = 'a';
    second = 'b';
    node = first << 8 + second;

    freqs[node] = 10;
    tree2.build(freqs);
    tree2.encode(node, bos2);
    bos2.flush();
    EXPECT_EQ(ss.get(), asciiVal);
}

TEST(HCTree2Tests, SMALL_TEST_DECODE_BIT) {
    HCTree2 tree1, tree2;
    vector<unsigned int> freqs(65535);

    string bitsStr = "00000000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis1(ss);

    // empty tree
    tree1.build(freqs);
    EXPECT_EQ(tree1.decode(bis1), ' ');

    // one-node tree
    unsigned char first, second;
    byte2 node;
    first = 'a';
    second = 'b';
    node = first << 8 + second;

    freqs[node] = 10;
    tree2.build(freqs);
    ss.str(ascii);
    BitInputStream bis2(ss);
    EXPECT_EQ(tree2.decode(bis2), node);
}

TEST(HCTree2Tests, SMALL_TEST_GETDISTINCTCHAR) {
    HCTree2 tree1, tree2;
    vector<unsigned int> freqs(65536);

    tree1.build(freqs);
    EXPECT_EQ(tree1.getDistinctChars(), 0);

    unsigned char first, second;
    byte2 node;
    first = 'a';
    second = 'b';
    node = first << 8 + second;

    freqs[node] = 10;
    tree2.build(freqs);
    EXPECT_EQ(tree2.getDistinctChars(), 1);
}

TEST(HCTree2Tests, SMALL_TEST_GETTREE) {
    HCTree2 tree1, tree2;
    vector<unsigned int> freqs(65536);

    stringstream ss;
    BitOutputStream bos(ss);

    tree1.build(freqs);
    string bitsStr = "00000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    tree1.getTree(bos);
    bos.flush();
    EXPECT_EQ(ss.get(), asciiVal);

    ss.str("");

    unsigned char first, second;
    byte2 node;
    first = 'a';
    second = 'b';
    node = first << 8 + second;

    freqs[node] = 10;
    tree2.build(freqs);
    bitsStr = "10000100";
    asciiVal = stoi(bitsStr, nullptr, 2);
    tree2.getTree(bos);
    bos.flush();
    EXPECT_EQ(ss.get(), asciiVal);
}

TEST(HCTree2Tests, TEST_RECONSTRUCT) {
    string bitsStr = "1000010000000000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ssi;
    ssi.str(ascii);
    BitInputStream bis(ssi);

    HCTree2* tree = new HCTree2();
    tree->reconstructTree(bis, 2);
}

TEST(HCNode2, TEST_PRINT) {
    // test for printing HCNode
    unsigned char first, second;
    byte2 node;
    first = 'a';
    second = 'b';
    node = first << 8 + second;

    HCNode2* hcnode = new HCNode2(node, 10);
    cout << *hcnode << endl;
    delete hcnode;
    EXPECT_TRUE(1);
}
