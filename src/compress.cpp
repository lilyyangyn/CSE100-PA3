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
#include "cxxopts.hpp"

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
void trueCompression(string inFileName, string outFileName) {
    vector<unsigned int> freqs(256);

    // open the input file
    ifstream inFile;
    inFile.open(inFileName);
    // read the input file
    unsigned char c;
    unsigned int total = 0;
    while (1) {
        c = inFile.get();
        if (inFile.eof()) break;
        freqs[c]++;
        total++;
    }

    // build HCTree
    HCTree* hctree = new HCTree();
    hctree->build(freqs);

    // open the output file
    ofstream outFile;
    outFile.open(outFileName);
    // prepare the bit output stream
    BitOutputStream bitOut(outFile);

    // write the header
    byte bit;
    // total number, can be up to 2ˆ32 = 4GB
    for (int i = 3; i > -1; i--) {
        bit = (total >> (8 * i)) & 255;
        outFile << bit;
    }
    // distinct characters
    byte count = 0;
    count += hctree->getDistinctChars();
    outFile << count;
    hctree->getTree(bitOut);

    // reset to read input file from beginning
    inFile.clear();
    inFile.seekg(0, ios::beg);
    // write encoded text
    while (1) {
        c = inFile.get();
        if (inFile.eof()) break;
        hctree->encode(c, bitOut);
        cout << c;
    }
    bitOut.flush();
    // close files
    inFile.close();
    outFile.close();

    // release memory
    delete hctree;
}

/* Main program that runs the compress */
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

    if (!FileUtils::isEmptyFile(inFileName)) {
        if (isAsciiOutput) {
            pseudoCompression(inFileName, outFileName);
        } else {
            trueCompression(inFileName, outFileName);
        }
    } else {
        ofstream outFile;
        outFile.open(outFileName);
        outFile.close();
    }
    return 0;
}