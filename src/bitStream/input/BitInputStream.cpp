/**
 * This file shows the implementation of BitInputStream class methods.
 * Declaration can be found in 'BitInputStream.hpp'
 *
 * Author: Yuening YANG
 * Email: y3yang@ucsd.edu
 */
#include "BitInputStream.hpp"

/* Fills the one byte buffer from input stream */
void BitInputStream::fill() {
    buf = in.get();
    nbits = 7;
}

/* Read next bit from the buffer. if the buffer has been
      already read, fill it */
unsigned int BitInputStream::readBit() {
    if (nbits == -1) {
        fill();
    }
    unsigned int nextbit = (buf >> nbits) & 1;
    nbits--;
    return nextbit;
}