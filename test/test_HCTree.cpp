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

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    // test normal encoding
    istringstream is("11");
    ASSERT_EQ(tree.decode(is), 'b');
    istringstream is2("10110");
    ASSERT_EQ(tree.decode(is2), 'a');
    EXPECT_EQ(tree.decode(is2), 'b');
    EXPECT_EQ(tree.decode(is2), 'c');
}

TEST_F(SimpleHCTreeFixture, TEST_GETDISTINCTCHAR) {
    EXPECT_EQ(tree.getDistinctChars(), 3);
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

TEST(HCTreeTests, SMALL_TEST_GETDISTINCTCHAR) {
    HCTree tree1, tree2;
    vector<unsigned int> freqs(256);

    tree1.build(freqs);
    EXPECT_EQ(tree1.getDistinctChars(), 0);

    freqs['a'] = 10;
    tree2.build(freqs);
    EXPECT_EQ(tree2.getDistinctChars(), 1);
}

TEST(HCNode, TEST_PRINT) {
    // test for printing HCNode
    HCNode* hcnode = new HCNode('a', 10);
    cout << *hcnode << endl;
    delete hcnode;
    EXPECT_TRUE(1);
}
