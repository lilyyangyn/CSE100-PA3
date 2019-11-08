/**
 * This file declares the BitInputStream class, including
 * the private variables and the methods
 *
 * Author: Yuening YANG, Shenlang Zhou
 * Email: y3yang@ucsd.edu
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** A class, instance of which is a bit input stream */
class BitInputStream {
  private:
    char buf;     // one byte buffer of bits
    int nbits;    // number of bits have been writen to buf
    istream& in;  // reference to the input stream to use

  public:
    /* constructor of BitInputStream */
    explicit BitInputStream(istream& is) : in(is), buf(0), nbits(-1){};

    /* Fills the one byte buffer from input stream */
    void fill();

    /* Read next bit from the buffer. if the buffer has been
      already read, fill it */
    unsigned int readBit();
};

#endif