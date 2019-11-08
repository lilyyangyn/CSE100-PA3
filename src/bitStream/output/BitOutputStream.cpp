/**
 * This file shows the implementation of BitOutputStream class methods.
 * Declaration can be found in 'BitOutputStream.hpp'
 *
 * Author: Yuening YANG, Shenlang Zhou
 * Email: y3yang@ucsd.edu
 */
#include "BitOutputStream.hpp"

/* Sends the buffer to the output stream, and then
      clear the buffer to allow further use. */
void BitOutputStream::flush() {
    out.put(buf);
    out.flush();
    buf = 0;
    nbits = 7;
}

/* Writes the least significant bit of the given int to the bit buffer.
      if the buffer is full, then flush it */
void BitOutputStream::writeBit(int i) {
    if (nbits == -1) {
        flush();
    }
    buf = buf + (i << nbits);
    nbits--;
}
