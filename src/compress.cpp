/**
 * This file implements a program to perform huffman encoding
 * on the input file and writes the output to the output file
 *
 * Author: Yuening YANG
 * Email: y3yang@ucsd.edu
 */
#include <fstream>
#include <iostream>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* add pseudo compression with ascii encoding and naive header
 * (checkpoint)
 *      params: names of the input file and the output file */
void pseudoCompression(string inFileName, string outFileName) {
    vector<unsigned int> freqs(256);

    // open the input file
    ifstream inFile;
    inFile.open(inFileName);
    // read the input file
    unsigned char c;
    while (1) {
        c = inFile.get();
        if (inFile.eof()) break;
        freqs[c]++;
    }

    // build HCTree
    HCTree* hctree = new HCTree();
    hctree->build(freqs);

    // open the output file
    ofstream outFile;
    outFile.open(outFileName, ios::out);

    // write the header
    for (int i = 0; i < 256; i++) {
        outFile << freqs[i] << endl;
    }

    // reset to read input file from beginning
    inFile.clear();
    inFile.seekg(0, ios::beg);
    // write encoded text
    while (1) {
        c = inFile.get();
        if (inFile.eof()) break;
        hctree->encode(c, outFile);
    }
    // close files
    inFile.close();
    outFile.close();

    // release memory
    delete hctree;
}

/* True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {}

/* Main program that runs the compress */
int main(int argc, char* argv[]) {
    const int NUM_ARG = 3;
    if (argc != NUM_ARG) {
        cout << "Invalid number of arguments.\n"
             << "Usage: ./compress <input filename> <output filename>" << endl;
        return -1;
    }
    if (!FileUtils::isValidFile(argv[1])) return -1;

    if (!FileUtils::isEmptyFile(argv[1])) {
        pseudoCompression(argv[1], argv[2]);
    } else {
        ofstream outFile;
        outFile.open(argv[2]);
        outFile.close();
    }
    return 0;
}