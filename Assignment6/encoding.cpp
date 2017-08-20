/**
 * @brief The following code involves the methods neccessary to compress, decompress,
 * encode, decode files and strings. The compression uses Huffman encoding algortihm.
 * The code uses abstract data structures such as maps, queues and trees in order to
 * perform Huffman's encoding algorithm.
 * SectionLeader: Ryan Kurohara
 * @author EFE ACER
 * @version 1.0
 */

#include "encoding.h"
#include "mymap.h"
#include "pqueue.h"
#include "filelib.h"

//constant decleration(s)
const int ASCII_OF_0 = 48; //used in encodeData function

/**
 * @brief buildFrequencyTable Builds a frequency table from the a given input stream containing
 * data.
 * @param input The given input stream whose data will be used to generate a frequency table.
 * @return The frequency table, with the frequencies of the characters in the input stream's data.
 */
MyMap buildFrequencyTable(istream& input) {
    MyMap freqTable;
    int byte = input.get();
    while (byte != -1) {
        if (freqTable.containsKey(byte)) {
            freqTable.put(byte, freqTable.get(byte) + 1);
        } else {
            freqTable.put(byte, 1);
        }
        byte = input.get();
    }
    freqTable.put(PSEUDO_EOF, 1);
    return freqTable;
}

/**
 * @brief buildEncodingTree Builds an encoding tree to from a given frequency table. The tree is
 * built according to the Huffman's algorithm and a priority queue is used in the steps.
 * @param freqTable The given frequency table, which contains the data for the tree.
 * @return The encoding tree built from Huffman's algorithm.
 */
HuffmanNode* buildEncodingTree(const MyMap& freqTable) {
    PriorityQueue<HuffmanNode*> frequencies;
    for (int key: freqTable.keys()) {
        HuffmanNode* node = new HuffmanNode(key, freqTable.get(key));
        frequencies.enqueue(node, node->count);
    }
    while (frequencies.size() != 1) {
        HuffmanNode* zero = frequencies.dequeue();
        HuffmanNode* one = frequencies.dequeue();
        HuffmanNode* root = new HuffmanNode(NOT_A_CHAR, zero->count + one->count, zero, one);
        frequencies.enqueue(root, root->count);
    }
    return frequencies.dequeue();
}

/**
 * @brief generateEncodingMap A recursive helper function used by the buildEncodingMap function.
 * Basically performs pre-order traversal on the encoding tree and adds the binary code of the data
 * , which meets the conditions to the map. Stores the binary code through the recursive calls.
 * @param encodingMap The reference to the encoding map.
 * @param node A pointer to the encoding tree.
 * @param binaryCode The stored binary code.
 */
void generateEncodingMap(Map<int, string> &encodingMap, HuffmanNode* node, string binaryCode) {
    if (node != nullptr) {
        if (node->character != NOT_A_CHAR) {
            encodingMap.put(node->character, binaryCode);
        }
        generateEncodingMap(encodingMap, node->zero, binaryCode + "0");
        generateEncodingMap(encodingMap, node->one, binaryCode + "1");
    }
}

/**
 * @brief buildEncodingMap Builds an encoding map, where every character in a given encoding tree
 * have a suitable Huffman binary code. Those binary codes are added to the encoding map by
 * traversing the given encoding tree with a call to the generateEncodingMap function.
 * @param encodingTree The given encoding tree which is used to build the encoding map.
 * @return The encoding map built from Huffman's algorithm.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    generateEncodingMap(encodingMap, encodingTree, "");
    return encodingMap;
}

/**
 * @brief encodeData Encodes a data using Huffman's algorithm.
 * @param input The input stream containing the data.
 * @param encodingMap The map storing Huffman binary codes for the data.
 * @param output The output stream, where the encoded data goes.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int byte = input.get();
    while (byte != -1) {
        string binaryCode = encodingMap.get(byte);
        for (int bit: binaryCode) {
            output.writeBit(bit - ASCII_OF_0);
        }
        byte = input.get();
    }
    for (int bit: encodingMap.get(PSEUDO_EOF)) {
        output.writeBit(bit - ASCII_OF_0);
    }
}

/**
 * @brief decodeData Decodes a data using Huffman's algorithm.
 * @param input The input stream containing the data.
 * @param encodingTree The tree used to perform reverse-Huffman encoding.
 * @param output The output stream, where the encoded data goes.
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    int bit = input.readBit();
    HuffmanNode* current = encodingTree;
    while (bit != -1) {
        if (current != nullptr) { //not neccessary but it's always good to prevent nullptrs
            if (current->character == PSEUDO_EOF) {
                return;
            } else if (current->character != NOT_A_CHAR) {
                output.put(current->character);
                current = encodingTree;
            }
            if (bit == 0) {
                current = current->zero;
            } else {
                current = current->one;
            }
        }
        bit = input.readBit();
    }
}

/**
 * @brief compress Encodes the data in a file and makes it smaller in size. Uses Huffman
 * encoding by calling several functions. Generates a compressed version of data.
 * @param input The input stream to be compressed.
 * @param output The compressed data's output stream.
 */
void compress(istream& input, obitstream& output) {
    MyMap freqTable = buildFrequencyTable(input);
    rewindStream(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    output << freqTable;
    encodeData(input, encodingMap, output);
}

/**
 * @brief decompress Decodes the data in a file and makes it larger in size. Used Huffman
 * decoding by calling several functions. Generates a decompressed version of data.
 * @param input The input stream to be decompressed.
 * @param output The decompressed data's output stream.
 */
void decompress(ibitstream& input, ostream& output) {
    MyMap freqTable;
    input >> freqTable;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
}

/**
 * @brief freeTree Frees and deletes all the memory associated with a given tree.
 * @param node The reference to the tree to be deleted.
 */
void freeTree(HuffmanNode* node) {
    if (node != nullptr) {
        HuffmanNode* trash = node;
        freeTree(node->zero);
        freeTree(node->one);
        delete trash;
        node = nullptr;
    }
}
