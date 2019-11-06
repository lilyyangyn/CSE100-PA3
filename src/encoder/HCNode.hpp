/**
 * This file declares and also implements the HCNode class,
 * including the private variables, the methods and their
 * implementation
 *
 * Author: Yuening Yang, Shenlang Zhou
 * Email: y3yang@ucsd.edu
 */
#ifndef HCNODE_HPP
#define HCNODE_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** A class, instances of which are nodes in an HCTree.
 */
class HCNode {
  public:
    unsigned int count;  // the freqency of the symbol
    byte symbol;         // byte in the file we're keeping track of
    HCNode* c0;          // pointer to '0' child
    HCNode* c1;          // pointer to '1' child
    HCNode* p;           // pointer to parent
    bool isZeroChild;

    /* Constructor that initialize a HCNode */
    HCNode(unsigned int count, byte symbol, HCNode* c0 = 0, HCNode* c1 = 0,
           HCNode* p = 0)
        : count(count),
          symbol(symbol),
          c0(c0),
          c1(c1),
          p(p),
          isZeroChild(false) {}
};

/* For printing an HCNode to an ostream. Possibly useful for debugging */
ostream& operator<<(ostream&, const HCNode&) __attribute__((weak));
ostream& operator<<(ostream& stm, const HCNode& n) {
    stm << "[" << n.count << "," << (int)(n.symbol) << "]";
    return stm;
}

/* Comparator of HCNode pointer. In priority queue, HCNode ptr with lower count
 * has higher priority, and if count is the same, HCNode ptr with larger symbol
 * has higher prioruty.
 */
struct HCNodePtrComp {
    /* a comparator of HCNode pointer, can be used to generate MinHeap
      params: 2 HCNodes to be compared
      return:  true if lhs > rhs, false otherwise*/
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        if (lhs->count == rhs->count) {
            // if count the same, the one with larger ASCII has higher priority
            return lhs->symbol < rhs->symbol;
        }
        return lhs->count > rhs->count;
    }
};
#endif  // HCNODE_HPP
