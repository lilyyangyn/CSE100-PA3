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
#include "cxxopts.hpp"

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
void trueDecompression(string inFileName, string outFileName) {
    // open the input file
    ifstream inFile;
    inFile.open(inFileName);
    BitInputStream bitIn(inFile);

    // open the output file
    ofstream outFile;
    outFile.open(outFileName);

    // read the header and reconstruct HCTree
    // get total number, 32 bits
    int total = 0, bit;
    for (int i = 0; i < 4; i++) {
        // bit = bitIn.readBit();
        bit = inFile.get();
        total = (total << 8) + bit;
    }

    // check empty file
    if (total == 0) {
        inFile.close();
        outFile.close();
        return;
    }

    // get distinct number
    int count = inFile.get() + 1;
    HCTree* hctree = new HCTree();
    hctree->reconstructTree(bitIn, count);

    // decode
    byte symbol = 0;
    int counts = 0;
    for (int i = 0; i < total; i++) {
        symbol = hctree->decode(bitIn);
        outFile << symbol;
    }
    // close files
    inFile.close();
    outFile.close();

    // release memory
    delete hctree;
}

/* Main program that runs the uncompress */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit strem",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h, help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }

    if (!FileUtils::isEmptyFile(argv[1])) {
        if (isAsciiOutput) {
            pseudoDecompression(inFileName, outFileName);
        } else {
            trueDecompression(inFileName, outFileName);
        }
    } else {
        ofstream outFile;
        outFile.open(outFileName);
        outFile.close();
    }
    return 0;
}
