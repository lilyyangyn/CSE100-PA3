/**
 * This file implements a program to perform decoding of huffman encoding
 * on the input file, and writes the output to the output file
 *
 * Author: Yuening YANG
 * Email: y3yang@ucsd.edu
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* Pseudo decompression with ascii encoding and naive header (checkpoint)
 */
void pseudoDecompression(string inFileName, string outFileName) {
    vector<unsigned int> freqs(256);

    // open the input file
    ifstream inFile;
    inFile.open(inFileName);
    // read the header
    int n;
    for (int i = 0; i < 256; i++) {
        freqs[i] = inFile.get() - '0';
        while ((n = inFile.get()) != '\n') {
            n = n - '0';
            freqs[i] = freqs[i] * 10 + n;
        }
    }

    // construct HCTree
    HCTree* hctree = new HCTree();
    hctree->build(freqs);

    // open the output file
    ofstream outFile;
    outFile.open(outFileName);

    // decode
    byte symbol;
    int counts = 0;
    for (int i = 0; i < 256; i++) {
        counts += freqs[i];
    }
    for (int i = 0; i < counts; i++) {
        symbol = hctree->decode(inFile);
        outFile << symbol;
    }
    // close files
    inFile.close();
    outFile.close();

    // release memory
    delete hctree;
}

/* True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {}

/* Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 3;
    if (argc != NUM_ARG) {
        cout << "Invalid number of arguments.\n"
             << "Usage: ./uncompress <input filename> <output filename>"
             << endl;
        return -1;
    }
    if (!FileUtils::isValidFile(argv[1])) return -1;

    if (!FileUtils::isEmptyFile(argv[1])) {
        pseudoDecompression(argv[1], argv[2]);
    } else {
        ofstream outFile;
        outFile.open(argv[2]);
        outFile.close();
    }
    return 0;
}
