/**
 * This file declares the BitOutputStream class, including
 * the private variables and the methods
 *
 * Author: Yuening YANG, Shenlang ZHOU
 * Email: y3yang@ucsd.edu
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** A class, instance of which is a bit output stream */
class BitOutputStream {
  private:
    char buf;      // one byte buffer of bits
    int nbits;     // number of bits have been writen to buf
    ostream& out;  // reference to the output stream to use

  public:
    /* constructor of BitOutputStream */
    explicit BitOutputStream(ostream& os) : out(os), buf(0), nbits(7){};

    /* Sends the buffer to the output stream, and then
      clear the buffer to allow further use. */
    void flush();

    /* Writes the least significant bit of the given int to the bit buffer.
      if the buffer is full, then flush it */
    void writeBit(int i);
};

#endif
