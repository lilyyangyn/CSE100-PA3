/**
 * This file declares and also implements the HCNode class,
 * including the private variables, the methods and their
 * implementation
 *
 * Author: Yuening Yang, Shenlang Zhou
 * Email: y3yang@ucsd.edu
 */
#ifndef HCNODE2_HPP
#define HCNODE2_HPP

#include <iostream>

typedef unsigned short byte2;

using namespace std;

/** A class, instances of which are nodes in an HCTree.
 */
class HCNode2 {
  public:
    unsigned int count;     // the freqency of the symbol
    unsigned short symbol;  // byte in the file we're keeping track of
    HCNode2* c0;             // pointer to '0' child
    HCNode2* c1;             // pointer to '1' child
    HCNode2* p;              // pointer to parent
    bool isZeroChild;

    /* Constructor that initialize a HCNode2 */
    HCNode2(unsigned int count, byte2 symbol, HCNode2* c0 = 0, HCNode2* c1 = 0,
           HCNode2* p = 0)
        : count(count),
          symbol(symbol),
          c0(c0),
          c1(c1),
          p(p),
          isZeroChild(false) {}
};

/* For printing an HCNode2 to an ostream. Possibly useful for debugging */
ostream& operator<<(ostream&, const HCNode2&) __attribute__((weak));
ostream& operator<<(ostream& stm, const HCNode2& n) {
    stm << "[" << n.count << "," << (int)(n.symbol) << "]";
    return stm;
}

/* Comparator of HCNode2 pointer. In priority queue, HCNode2 ptr with lower count
 * has higher priority, and if count is the same, HCNode2 ptr with larger symbol
 * has higher prioruty.
 */
struct HCNode2PtrComp {
    /* a comparator of HCNode2 pointer, can be used to generate MinHeap
      params: 2 HCNodes to be compared
      return:  true if lhs > rhs, false otherwise*/
    bool operator()(HCNode2*& lhs, HCNode2*& rhs) const {
        if (lhs->count == rhs->count) {
            // if count the same, the one with larger ASCII has higher priority
            return lhs->symbol < rhs->symbol;
        }
        return lhs->count > rhs->count;
    }
};
#endif  // HCNODE_HPP
