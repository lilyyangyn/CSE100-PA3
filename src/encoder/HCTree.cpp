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
    // check if no empty input
    if (pq.size() == 0) {
        return;
    }

    // set EOF
    HCNode* end = new HCNode(0, EOF);
    leaves[256] = end;
    pq.push(end);

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
        p = new HCNode(ptr1->count + ptr2->count, ptr2->symbol);
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
}

/* return the number of leaves of HCTree */
unsigned int HCTree::getDistinctChars() {
    unsigned int count = 0;
    for (int i = 0; i < 257; i++) {
        if (leaves[i] != 0) {
            count++;
        }
    }
    return count;
}

/* Write the encoding bits of given symbol to the given BitOutputStream. For
      this function to work, must first build the tree
        symbol: a symbol to be encoded
        out: the output stream, should be passed by reference */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    if (root == 0) {
        return;
    }

    HCNode* ptr;
    if (symbol == (byte)EOF) {
        ptr = leaves[256];
    } else {
        ptr = leaves[symbol];
    }
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

    HCNode* ptr;
    if (symbol == (byte)EOF) {
        ptr = leaves[256];
    } else {
        ptr = leaves[symbol];
    }
    string buffer = "";
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

/* Helper method for getTree, in order traverse the tree */
void HCTree::getTreeHelper(HCNode* ptr, BitOutputStream& out) const {
    if (ptr == 0) {
        return;
    }

    if (ptr->c0 != 0 && ptr->c0->c0 != 0) {
        out.writeBit(0);
    }
    getTreeHelper(ptr->c0, out);

    if (ptr->c0 == 0 && ptr->c1 == 0) {
        out.writeBit(1);
        for (int i = 7; i > -1; i--) {
            out.writeBit(((ptr->symbol >> i) & 1));
        }
        return;
    } else {
        if (ptr->c1->c1 != 0) {
            out.writeBit(0);
        }
    }
    getTreeHelper(ptr->c1, out);
}

/* Helper method for getTree, in order traverse the tree */
void HCTree::getTreeHelper(HCNode* ptr, ostream& out) const {
    if (ptr == 0) {
        return;
    }

    if (ptr->c0 != 0 && ptr->c0->c0 != 0) {
        out << '0';
    }
    getTreeHelper(ptr->c0, out);

    if (ptr->c0 == 0 && ptr->c1 == 0) {
        out << '1';
        out << ptr->symbol;
        return;
    } else {
        if (ptr->c1->c1 != 0) {
            out << '0';
        }
    }
    getTreeHelper(ptr->c1, out);
}

/* get the tree structure. can be used to reconstruct the tree */
void HCTree::getTree(BitOutputStream& out) const { getTreeHelper(root, out); };

/* get the tree structure. can be used to reconstruct the tree */
void HCTree::getTree(ostream& out) const { getTreeHelper(root, out); };

/* reconstruct the tree according to the encoding header */
void HCTree::reconstructTree(BitInputStream& in, int total) {}

/* reconstruct the tree according to the encoding header */
void HCTree::reconstructTree(ifstream& in, int total) {
    char c;
    int count = 0;
    c = in.get();
    if (c == ' ')
        while (count < total) {
        }
}