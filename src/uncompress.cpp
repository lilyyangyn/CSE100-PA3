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
    // read the header and reconstruct HCTree
    int count = 0;
    int n = inFile.get() - '0';
    while (n != '\n' - '0') {
        count = count * 10 + n;
        n = inFile.get() - '0';
    }
    HCTree* hctree = new HCTree();
    hctree->reconstructTree(inFile, count);

    // open the output file
    ofstream outFile;
    outFile.open(outFileName);

    // decode
    byte symbol = 'a';
    int counts = 0;
    while (1) {
        symbol = hctree->decode(inFile);
        if (symbol == (byte)EOF) break;
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
    vector<unsigned int> freqs(256);

    // open the input file
    ifstream inFile;
    inFile.open(inFileName);
    BitInputStream bitIn(inFile);
    // read the header and reconstruct HCTree
    int count = inFile.get() - 0;
    HCTree* hctree = new HCTree();
    hctree->reconstructTree(bitIn, count);

    // open the output file
    ofstream outFile;
    outFile.open(outFileName);

    // decode
    byte symbol = 0;
    int counts = 0;
    while (1) {
        // cout << "symbol" << endl;
        symbol = hctree->decode(bitIn);
        // cout << symbol << endl;
        if (symbol == (byte)EOF) break;
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
