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
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "10");
    os.str("");
    tree.encode('c', os);
    ASSERT_EQ(os.str(), "0");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("11");
    ASSERT_EQ(tree.decode(is), 'b');
    istringstream is2("10110");
    ASSERT_EQ(tree.decode(is2), 'a');
    EXPECT_EQ(tree.decode(is2), 'b');
    EXPECT_EQ(tree.decode(is2), 'c');
}
