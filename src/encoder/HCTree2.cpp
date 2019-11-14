/**
 * This file shows the implementation details of HCTree2 class
 * methods, which are declared in 'HCTree2.hpp' file.
 *
 * Author: Yuening YANG
 * Email: y3yang@ucsd.edu
 */
#include "HCTree2.hpp"

/* Destructor, automatically call it to avoid memory leak */
HCTree2::~HCTree2() {
    // since deleteAll() will delete all nodes, no need to free
    // memory for the pointers stored in leaves
    deleteAll(root);
}

/* Build the HCTree2 from the given frequency vector
      param: a vector contains the frequency of charactors to be encoded */
void HCTree2::build(const vector<unsigned int>& freqs) {
    priority_queue<HCNode2*, vector<HCNode2*>, HCNode2PtrComp> pq;
    HCNode2* ptr;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            // create new node
            ptr = new HCNode2(freqs[i], byte2(i));
            byte2 symbol = byte2(i);
            // store it to the leaves vector
            leaves[i] = ptr;
            // push into minHeap to prepare building the tree
            pq.push(ptr);
        } else {
            leaves[i] = 0;
        }
    }
    // check if no empty input
    if (pq.size() == 0) {
        return;
    } else if (pq.size() == 1) {
        root = pq.top();
        return;
    }

    // the tree is build in this way:
    // left child < right child according to count (alphabet when counts equal)
    // parent use the left child's symbol as its symbol
    HCNode2 *ptr1, *ptr2, *p;
    while (pq.size() > 1) {
        ptr1 = pq.top();
        pq.pop();
        ptr2 = pq.top();
        pq.pop();
        // build the parent node
        p = new HCNode2(ptr1->count + ptr2->count, ptr2->symbol);
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

/* return the number of leaves of HCTree2 */
unsigned int HCTree2::getDistinctChars() {
    if (root == 0) {
        return 0;
    }
    unsigned int count = 0;
    for (int i = 0; i < 65536; i++) {
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
void HCTree2::encode(byte2 symbol, BitOutputStream& out) const {
    if (root == 0) {
        return;
    }
    if (root->c0 == 0 && root->c1 == 0) {
        out.writeBit(0);
        return;
    }

    HCNode2* ptr = leaves[symbol];
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

/* Get the sequence of bits from BitInputStream, decode, then return
      param:
        in: the input stream, should be passed by reference
      return:
        the decoded symbol */
byte2 HCTree2::decode(BitInputStream& in) const {
    if (root == 0) {
        return ' ';
    }
    if (root->c0 == 0 && root->c1 == 0) {
        return root->symbol;
    }
    unsigned int bit;
    HCNode2* ptr = root;
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

/* Helper function for destructor. Recursively deletes all the nodes.
        argument: a pointer pointing to the root of the subtree to be deleted.
     */
void HCTree2::deleteAll(HCNode2* ptr) {
    if (ptr == 0) {
        return;
    }
    deleteAll(ptr->c0);
    deleteAll(ptr->c1);
    delete ptr;
}

/* Helper method for getTree, in order traverse the tree */
void HCTree2::getTreeHelper(HCNode2* ptr, BitOutputStream& out) const {
    if (ptr == 0) {
        return;
    }

    if (ptr->c0 != 0 && ptr->c0->c0 != 0) {
        out.writeBit(0);
    }
    getTreeHelper(ptr->c0, out);

    if (ptr->c0 == 0 && ptr->c1 == 0) {
        out.writeBit(1);
        for (int i = 15; i > -1; i--) {
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

/* get the tree structure. can be used to reconstruct the tree */
void HCTree2::getTree(BitOutputStream& out) const {
    if (root == 0) {
        return;
    }
    if (root->c0 == 0 && root->c1 == 0) {
        for (int i = 15; i > -1; i--) {
            out.writeBit(((root->symbol >> i) & 1));
        }
    }
    getTreeHelper(root, out);
};

/* reconstruct the tree according to the encoding header */
void HCTree2::reconstructTree(BitInputStream& in, int total) {
    if (total == 0) {
        return;
    }
    if (total == 1) {
        byte2 character = 0;
        for (int i = 15; i > -1; i--) {
            character = character + (in.readBit() << i);
        }
        root = new HCNode2(0, character);
        leaves[character] = root;
        return;
    }

    int c;
    byte2 character;
    int count = 0;
    root = new HCNode2(0, ' ');
    HCNode2* ptr = root;

    c = in.readBit();
    HCNode2* leaf = 0;
    while (count < total) {
        // bit 0
        if (c == 0) {
            // create node
            if (ptr->c0 == 0) {
                ptr->c0 = new HCNode2(0, ' ');
                ptr->c0->p = ptr;
                ptr = ptr->c0;
            } else {
                ptr->c1 = new HCNode2(0, ' ');
                ptr->c1->p = ptr;
                ptr = ptr->c1;
            }
        }
        // bit 1
        if (c == 1) {
            // create leaf
            // get the character
            character = 0;
            for (int i = 15; i > -1; i--) {
                character = character + (in.readBit() << i);
            }
            count++;
            leaf = new HCNode2(0, character);
            // add to the leaves list
            leaves[character] = leaf;

            unsigned char first = ((character >> 8) & 255);
            unsigned char second = (character & 255);

            // construct the tree
            if (ptr->c0 == 0) {
                ptr->c0 = leaf;
                ptr->c0->p = ptr;

                // now ptr pointing to the parent of the leaf
            } else {
                ptr->c1 = leaf;
                ptr->c1->p = ptr;
                if (count == total) break;
                while (ptr != 0 && ptr->c1 != 0) {
                    ptr = ptr->p;
                }
            }
        }
        // get next
        c = in.readBit();
    }
}
