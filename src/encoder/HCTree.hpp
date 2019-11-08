/**
 * This file declares the structure of HCTree class,
 * including its private variables and public methods
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu, shzhou@ucsd.edu
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

/** This class defines the Huffman-encoding Tree */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    /* Constructor that initialize a HCTree */
    HCTree() : root(0) {
        leaves.reserve(256);
        fill(leaves.begin(), leaves.end(), nullptr);
    }

    /* Destructor, automatically call it to avoid memory leak */
    ~HCTree();

    /* Build the HCTree from the given frequency vector
      param: a vector contains the frequency of charactors to be encoded */
    void build(const vector<unsigned int>& freqs);

    /* Write the encoding bits of given symbol to the given BitOutputStream. For
      this function to work, must first build the tree
        symbol: a symbol to be encoded
        out: the output stream, should be passed by reference */
    void encode(byte symbol, BitOutputStream& out) const;

    /* Write the encoding bits of given symbol to ostream. For
      this function to work, must first build the tree
      params:
        symbol: a symbol to be encoded
        out: the output stream, should be passed by reference */
    void encode(byte symbol, ostream& out) const;

    /* Get the sequence of bits from BitInputStream, decode, then return
      param:
        in: the input stream, should be passed by reference
      return:
        the decoded symbol */
    byte decode(BitInputStream& in) const;

    /* Get the sequence of bits from istream, decode, then return
      param:
        in: the input stream, should be passed by reference
      return:
        the decoded symbol */
    byte decode(istream& in) const;

  private:
    /* Helper function for destructor. Recursively deletes all the nodes.
        argument: a pointer pointing to the root of the subtree to be deleted.
     */
    void deleteAll(HCNode* ptr);
};

#endif  // HCTREE_HPP
