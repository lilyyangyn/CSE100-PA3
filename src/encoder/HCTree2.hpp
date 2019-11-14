/**
 * This file declares the structure of HCTree2 class,
 * including its private variables and public methods
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu, shzhou@ucsd.edu
 */
#ifndef HCTREE2_HPP
#define HCTREE2_HPP

#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode2.hpp"

using namespace std;

/** This class defines the Huffman-encoding Tree */
class HCTree2 {
  private:
    HCNode2* root;            // the root of HCTree2
    vector<HCNode2*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    /* Constructor that initialize a HCTree2 */
    HCTree2() : root(0) { leaves.reserve(65536); }

    /* Destructor, automatically call it to avoid memory leak */
    ~HCTree2();

    /* Build the HCTree from the given frequency vector
      param: a vector contains the frequency of charactors to be encoded */
    void build(const vector<unsigned int>& freqs);

    /* return the number of leaves of HCTree */
    unsigned int getDistinctChars();

    /* Write the encoding bits of given symbol to the given BitOutputStream. For
      this function to work, must first build the tree
        symbol: a symbol to be encoded
        out: the output stream, should be passed by reference */
    void encode(byte2 symbol, BitOutputStream& out) const;

    /* Get the sequence of bits from BitInputStream, decode, then return
      param:
        in: the input stream, should be passed by reference
      return:
        the decoded symbol */
    byte2 decode(BitInputStream& in) const;

    /* get the tree structure. can be used to reconstruct the tree */
    void getTree(BitOutputStream& out) const;

    /* reconstruct the tree according to the encoding header */
    void reconstructTree(BitInputStream& in, int total);

  private:
    /* Helper function for destructor. Recursively deletes all the nodes.
        argument: a pointer pointing to the root of the subtree to be deleted.
     */
    void deleteAll(HCNode2* ptr);

    /* Helper method for getTree, in order traverse the tree */
    void getTreeHelper(HCNode2* ptr, BitOutputStream& out) const;
};

#endif  // HCTREE_HPP
