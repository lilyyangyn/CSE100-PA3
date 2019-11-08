/**
 * This file shows the implementation details of HCTree class
 * methods, which are declared in 'HCTree.hpp' file.
 *
 * Author: Yuening YANG
 * Email: y3yang@ucsd.edu
 */
#include "HCTree.hpp"

/* Destructor, automatically call it to avoid memory leak */
HCTree::~HCTree() {
    // since deleteAll() will delete all nodes, no need to free
    // memory for the pointers stored in leaves
    deleteAll(root);
}

/* Build the HCTree from the given frequency vector
      param: a vector contains the frequency of charactors to be encoded */
void HCTree::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    HCNode* ptr;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            // create new node
            ptr = new HCNode(freqs[i], char(i));
            // store it to the leaves vector
            leaves[i] = ptr;
            // push into minHeap to prepare building the tree
            pq.push(ptr);
        }
    }

    if (pq.size() == 0) {
        return;
    }

    // the tree is build in this way:
    // left child < right child according to count (alphabet when counts equal)
    // parent use the left child's symbol as its symbol
    HCNode *ptr1, *ptr2, *p;
    while (pq.size() > 1) {
        ptr1 = pq.top();
        pq.pop();
        ptr2 = pq.top();
        pq.pop();
        // build the parent node
        p = new HCNode(ptr1->count + ptr2->count, ptr1->symbol);
        p->c0 = ptr1;
        ptr1->p = p;
        ptr1->isZeroChild = true;
        p->c1 = ptr2;
        ptr2->p = p;
        ptr2->isZeroChild = false;
        // push the parent node back
        pq.push(p);
    }
    // set root
    root = pq.top();
    pq.pop();
}

/* Write the encoding bits of given symbol to the given BitOutputStream. For
      this function to work, must first build the tree
        symbol: a symbol to be encoded
        out: the output stream, should be passed by reference */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    if (root == 0) {
        return;
    }
    if (root->c0 == 0 && root->c1 == 0) {
        out.writeBit(0);
        return;
    }

    HCNode* ptr = leaves[symbol];
    string buffer = "";
    while (ptr != root) {
        if (ptr->isZeroChild) {
            buffer = '0' + buffer;
        } else {
            buffer = '1' + buffer;
        }
        ptr = ptr->p;
    }
    for (int i = 0; i < buffer.length(); i++) {
        out.writeBit(buffer[i] - '0');
    }
}

/* Write the encoding bits of given symbol to ostream. For
      this function to work, must first build the tree
      params:
        symbol: a symbol to be encoded
        out: the output stream, should be passed by reference */
void HCTree::encode(byte symbol, ostream& out) const {
    if (root == 0) {
        return;
    }
    if (root->c0 == 0 && root->c1 == 0) {
        // if only one symbol in the tree, output '0'
        out << '0';
        return;
    }

    string buffer = "";
    HCNode* ptr = leaves[symbol];
    while (ptr != root) {
        if (ptr->isZeroChild) {
            buffer = '0' + buffer;
        } else {
            buffer = '1' + buffer;
        }
        ptr = ptr->p;
    }
    out << buffer.c_str();
}

/* Get the sequence of bits from BitInputStream, decode, then return
      param:
        in: the input stream, should be passed by reference
      return:
        the decoded symbol */
byte HCTree::decode(BitInputStream& in) const {
    if (root == 0) {
        return ' ';
    }
    if (root->c0 == 0 && root->c1 == 0) {
        return root->symbol;
    }
    unsigned int bit;
    HCNode* ptr = root;
    while (ptr != 0 && ptr->c0 != 0 && ptr->c1 != 0) {
        bit = in.readBit();
        // out << bit;
        if (bit == 0) {
            ptr = ptr->c0;
        } else if (bit == 1) {
            ptr = ptr->c1;
        }
    }
    return ptr->symbol;
}

/* Get the sequence of bits from istream, decode, then return.
    For this function to work, must first build the tree
      param:
        in: the input stream, should be passed by reference
      return:
        the decoded symbol */
byte HCTree::decode(istream& in) const {
    if (root == 0) {
        return ' ';
    }
    if (root->c0 == 0 && root->c1 == 0) {
        return root->symbol;
    }
    char c;
    HCNode* ptr = root;
    while (ptr != 0 && ptr->c0 != 0 && ptr->c1 != 0) {
        c = in.get();
        if (c == '0') {
            ptr = ptr->c0;
        } else if (c == '1') {
            ptr = ptr->c1;
        }
    }
    return ptr->symbol;
}

/* Helper function for destructor. Recursively deletes all the nodes.
        argument: a pointer pointing to the root of the subtree to be deleted.
     */
void HCTree::deleteAll(HCNode* ptr) {
    if (ptr == 0) {
        return;
    }
    deleteAll(ptr->c0);
    deleteAll(ptr->c1);
    delete ptr;
}
