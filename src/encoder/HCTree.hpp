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
#include <map>
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
    map<int, string> codes;

  public:
    /* Constructor that initialize a HCTree */
    HCTree() : root(0) { leaves.reserve(257); }

    /* Destructor, automatically call it to avoid memory leak */
    ~HCTree();

    /* Build the HCTree from the given frequency vector
      param: a vector contains the frequency of charactors to be encoded */
    void build(const vector<unsigned int>& freqs);

    /* return the number of leaves of HCTree */
    unsigned int getDistinctChars();

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

    /* get the tree structure. can be used to reconstruct the tree */
    void getTree(BitOutputStream& out) const;

    /* get the tree structure. can be used to reconstruct the tree */
    void getTree(ostream& out) const;

    /* reconstruct the tree according to the encoding header */
    void reconstructTree(BitInputStream& in, int total);

    /* reconstruct the tree according to the encoding header */
    void reconstructTree(ifstream& in, int total);

  private:
    /* Helper function for destructor. Recursively deletes all the nodes.
        argument: a pointer pointing to the root of the subtree to be deleted.
     */
    void deleteAll(HCNode* ptr);

    /* Helper method for getTree, in order traverse the tree */
    void getTreeHelper(HCNode* ptr, BitOutputStream& out) const;

    /* Helper method for getTree, in order traverse the tree */
    void getTreeHelper(HCNode* ptr, ostream& out) const;
};

#endif  // HCTREE_HPP
