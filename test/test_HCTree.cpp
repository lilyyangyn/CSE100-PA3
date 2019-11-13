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
#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 2;
        freqs['b'] = 3;
        freqs['c'] = 5;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    // test normal encoding
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "10");
    // clear ostream
    os.str("");
    tree.encode('c', os);
    ASSERT_EQ(os.str(), "0");
    os.str("");
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE_BIT) {
    // test normal encoding
    stringstream ss;
    BitOutputStream bos(ss);
    tree.encode('a', bos);
    bos.flush();
    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    // test normal encoding
    istringstream is("11");
    ASSERT_EQ(tree.decode(is), 'b');
    istringstream is2("10110");
    ASSERT_EQ(tree.decode(is2), 'a');
    EXPECT_EQ(tree.decode(is2), 'b');
    EXPECT_EQ(tree.decode(is2), 'c');
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE_BIT) {
    // test normal encoding
    string bitsStr = "10110000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis(ss);
    ASSERT_EQ(tree.decode(bis), 'a');
    EXPECT_EQ(tree.decode(bis), 'b');
    EXPECT_EQ(tree.decode(bis), 'c');
}

TEST_F(SimpleHCTreeFixture, TEST_GETDISTINCTCHAR) {
    EXPECT_EQ(tree.getDistinctChars(), 3);
}

TEST_F(SimpleHCTreeFixture, TEST_GETTREE) {
    stringstream ss;
    BitOutputStream bos(ss);

    string bitsStr = "10110001";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    tree.getTree(bos);
    EXPECT_EQ(ss.get(), asciiVal);
}

/* Empty Tree & One-Node Tree Tests */
TEST(HCTreeTests, SMALL_TEST_ENCODE) {
    HCTree tree1, tree2;
    vector<unsigned int> freqs(256);

    ostringstream os;
    // empty tree
    tree1.build(freqs);
    tree1.encode(' ', os);
    EXPECT_EQ(os.str().size(), 0);

    os.str("");
    // one-node tree
    freqs['\n'] = 10;
    tree2.build(freqs);
    tree2.encode('\n', os);
    EXPECT_EQ(os.str(), "0");
}

TEST(HCTreeTests, SMALL_TEST_ENCODE_BIT) {
    HCTree tree1, tree2;
    vector<unsigned int> freqs(256);

    stringstream ss;
    BitOutputStream bos1(ss);
    string bitsStr = "00000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    // empty tree
    tree1.build(freqs);
    tree1.encode(' ', bos1);
    bos1.flush();
    EXPECT_EQ(ss.get(), asciiVal);

    ss.str("");
    BitOutputStream bos2(ss);
    // one-node tree
    freqs['\n'] = 10;
    tree2.build(freqs);
    tree2.encode('\n', bos2);
    bos2.flush();
    EXPECT_EQ(ss.get(), asciiVal);
}

TEST(HCTreeTests, SMALL_TEST_DECODE) {
    HCTree tree1, tree2;
    vector<unsigned int> freqs(256);

    // empty tree
    istringstream is1("");
    tree1.build(freqs);
    EXPECT_EQ(tree1.decode(is1), ' ');

    // one-node tree
    freqs['\n'] = 10;
    tree2.build(freqs);
    istringstream is2("0");
    EXPECT_EQ(tree2.decode(is2), '\n');
}

TEST(HCTreeTests, SMALL_TEST_DECODE_BIT) {
    HCTree tree1, tree2;
    vector<unsigned int> freqs(256);

    string bitsStr = "00000000";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ss;
    ss.str(ascii);
    BitInputStream bis1(ss);

    // empty tree
    tree1.build(freqs);
    EXPECT_EQ(tree1.decode(bis1), ' ');

    // one-node tree
    freqs['\n'] = 10;
    tree2.build(freqs);
    ss.str(ascii);
    BitInputStream bis2(ss);
    EXPECT_EQ(tree2.decode(bis2), '\n');
}

TEST(HCTreeTests, SMALL_TEST_GETDISTINCTCHAR) {
    HCTree tree1, tree2;
    vector<unsigned int> freqs(256);

    tree1.build(freqs);
    EXPECT_EQ(tree1.getDistinctChars(), 0);

    freqs['a'] = 10;
    tree2.build(freqs);
    EXPECT_EQ(tree2.getDistinctChars(), 1);
}

TEST(HCTreeTests, SMALL_TEST_GETTREE) {
    HCTree tree1, tree2;
    vector<unsigned int> freqs(256);

    stringstream ss;
    BitOutputStream bos(ss);

    tree1.build(freqs);
    string bitsStr = "00000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    tree1.getTree(bos);
    bos.flush();
    EXPECT_EQ(ss.get(), asciiVal);

    ss.str("");
    freqs['a'] = 10;
    tree2.build(freqs);
    bitsStr = "01100001";
    asciiVal = stoi(bitsStr, nullptr, 2);
    tree2.getTree(bos);
    bos.flush();
    EXPECT_EQ(ss.get(), asciiVal);
}

TEST(HCTreeTests, TEST_RECONSTRUCT) {
    string bitsStr = "1011000110101100001101100010";
    string ascii = string(1, stoi(bitsStr, nullptr, 2));

    stringstream ssi;
    ssi.str(ascii);
    BitInputStream bis(ssi);

    HCTree* tree = new HCTree();
    tree->reconstructTree(bis, 3);
}

TEST(HCNode, TEST_PRINT) {
    // test for printing HCNode
    HCNode* hcnode = new HCNode('a', 10);
    cout << *hcnode << endl;
    delete hcnode;
    EXPECT_TRUE(1);
}
