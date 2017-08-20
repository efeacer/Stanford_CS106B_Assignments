/**
  * INVOLVES EXTRA FEATURES
  * This program is a console based random text generator. The program code generates random text
  * from the information on a file. The generated sounds just like the author of the input text
  * because random text generation works like a Markov chain that each element is placed
  * according to its weighted probability. The code below involves functions and variables to
  * store and produce text.
  * Extensions:
  * Full sentence generation: changed getNGramMap and printRandomText functions.
  * @author EFE ACER
  * CS106B - Section Leader: Ryan Kurohara
  */

//necessary includes
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "map.h"
#include "vector.h"
#include "random.h"

using namespace std;

//constant declerations for further editing
const string INTRO = "Welcome to CS 106B Random Writer ('N-Grams').\n"
                     "This program makes random text based on a document.\n"
                     "Give me an input file and an 'N' value for groups\n"
                     "of words, and I'll create random text for you.\n\n";
const string PROMPT_FILE = "Input file name? ";
const string PROMPT_N = "Value of N? ";
const string PROMPT_RANDOM_WORD_NUMBER = "# of random words to generate (0 to quit)? ";
const string N_ERROR = "N must be 2 or greater.\n";
const string RANDOM_WORD_NUMBER_ERROR = "Must be at least 4 words.\n\n";
const string FILE_ERROR = "Unable to open that file.  Try again.\n";

//function declerations
void promptFile(string &file);
void promptN(int &N);
void promptRandomWordNumber(int &randomWordNumber);
Vector<string> getWords(string &file);
Map<Vector<string>, Vector<string>> getNGramMap(Vector<string> &words, int &N, Vector<Vector<string> > &startWords, Vector<Vector<string> > &endWords);
void printVector(Vector<string> &vec);
void printRandomText(Map<Vector<string>, Vector<string>> &NGramMap, int &randomWordNumber, int &N, Vector<Vector<string> > &startWords, Vector<Vector<string> > &endWords);

//main function
int main() {
    cout << INTRO; //displaying the intro welcome message
    string file;
    promptFile(file);
    Vector<string> words = getWords(file); //storing the words in the file
    int N; //asking for N
    promptN(N);
    Vector<Vector<string>> startWords, endWords;
    Map<Vector<string>, Vector<string>> NGramMap = getNGramMap(words, N, startWords, endWords); //storing the map
    cout << endl;
    int randomWordNumber;
    do {
        promptRandomWordNumber(randomWordNumber);
        if (randomWordNumber != 0) {
            printRandomText(NGramMap, randomWordNumber, N, startWords, endWords);
            cout << endl;
        }
    } while (randomWordNumber != 0);
    cout << "Exiting." << endl;
    return 0;
}

/**
 * @brief promptFile Asks for a valid file name. Prints error messages if neccessary.
 * @param file A reference to the file name's string.
 */
void promptFile(string &file) {
    do { //promting a file and processing it
        file = getLine(PROMPT_FILE);
        if (!isFile(file)) {
            cout << FILE_ERROR;
        }
    } while (!isFile(file));
}

/**
 * @brief promptN Asks for a valid N. Prints error messages if neccessary.
 * @param N A reference to the integer N.
 */
void promptN(int &N) {
    do { //asking for a valid value for N
        N = getInteger(PROMPT_N);
        if (N < 2) {
            cout << N_ERROR;
        }
    } while (N < 2);
}

/**
 * @brief promptRandomWordNumber Asks for a valid number for the random words. Prints
 * error messages if necessary.
 * @param randomWordNumber A reference to the integer storing the number of random
 * words.
 */
void promptRandomWordNumber(int &randomWordNumber) {
    do {
        randomWordNumber = getInteger(PROMPT_RANDOM_WORD_NUMBER);
        if (randomWordNumber != 0 && randomWordNumber < 4) {
            cout << RANDOM_WORD_NUMBER_ERROR;
        }
    } while (randomWordNumber != 0 && randomWordNumber < 4);
}

/**
 * @brief getWords Returns the words in a file as a Vector, reading one word at a time
 * @param file The file to read.
 * @return The collection (Vector) containing the words in the file.
 */
Vector<string> getWords(string &file) {
    ifstream input;
    openFile(input, file);
    Vector<string> words;
    string word;
    while (input >> word) {
        words.add(word);
    }
    return words;
}

/**
 * @brief getNGramMap Returns a Map, where each element is placed according to
 * its weighted probability (A Markov chain)
 * @param words The Vector containing all the words needed to generate the Map.
 * @param N is the number indicating the length of the Vectors in the
 * keys Vector of the Map.
 * @param startWords the words that start the sentences.
 * @param endWords the words that end the sentences.
 * @return The Map that is containing all the words and the probability information
 * (frequencies) needed to generate random text.
 */
Map<Vector<string>, Vector<string>> getNGramMap(Vector<string> &words, int &N, Vector<Vector<string>> &startWords,
                                                Vector<Vector<string>> &endWords) {
    Map<Vector<string>, Vector<string>> NGramMap;
    Vector<string> window;
    Vector<string> values;
    for (int i = 0; i < words.size(); i++) {
        for (int j = i; j < i + N - 1; j++) {
            if (j >= words.size()) { //the case for wrapping
                window.add(words.get(j % words.size()));
            }
            else {
                window.add(words.get(j));
            }
        }
        if (NGramMap.containsKey(window)) {
            values = NGramMap.get(window);
        }
        else {
            values.clear();
        }
        if (i + N - 1 >= words.size()) {
            values.add(words.get((i + N - 1) % words.size())); //the case for wrapping
            NGramMap.put(window, values);
        }
        else {
            values.add(words.get(i + N - 1));
            NGramMap.put(window, values);
        }
        if (charToString(window.get(0)[0]) == toUpperCase(charToString(window.get(0)[0]))) {
            startWords.add(window);
        }
        char check = window.get(window.size() - 1)[window.get(window.size() - 1).length() - 1];
        if (check == '.' || check == '!' || check == '?') {
            endWords.add(window);
        }
        window.clear();
    }
    return NGramMap;
}

/**
 * @brief printVector Prints a Vector as desired.
 * @param vec The Vector to print.
 */
void printVector(Vector<string> &vec) {
    for (string word: vec) {
        cout << word << " ";
    }
}

/**
 * @brief printRandomText Prints a random text using the Map containg the words according to
 * their frequencies.
 * @param NGramMap A reference to the Map, which contains words and information.
 * @param randomWordNumber Number of random words to be generated.
 * @param startWords the words that start the sentences.
 * @param endWords the words that end the sentences.
 * @param N The number determining the similarity between the actual text and the random text.
 */
void printRandomText(Map<Vector<string>, Vector<string>> &NGramMap, int &randomWordNumber, int &N,
                     Vector<Vector<string>> &startWords, Vector<Vector<string>> &endWords) {
    if (!startWords.isEmpty() && !endWords.isEmpty())
    {
        Vector<string> vec = startWords.get(randomInteger(0, startWords.size() - 1));
        printVector(vec);
        string value;
        for (int i = N - 1; i < randomWordNumber; i++) {
            value = NGramMap.get(vec).get(randomInteger(0, NGramMap.get(vec).size() - 1));
            cout << value << " ";
            vec.remove(0);
            vec.add(value);
        }
        char check = vec.get(vec.size() - 1)[vec[vec.size() - 1].length() - 1];
        while (check != '.' && check != '!' && check != '?') {
            value = NGramMap.get(vec).get(randomInteger(0, NGramMap.get(vec).size() - 1));
            cout << value << " ";
            vec.remove(0);
            vec.add(value);
            check = vec.get(vec.size() - 1)[vec[vec.size() - 1].length() - 1];
        }
        cout << endl;
    }
    else {
        Vector<string> vec = NGramMap.keys().get(randomInteger(0, NGramMap.keys().size() - 1));
        cout << "... ";
        printVector(vec);
        string value;
        for (int i = N - 1; i < randomWordNumber; i++) {
            value = NGramMap.get(vec).get(randomInteger(0, NGramMap.get(vec).size() - 1));
            cout << value << " ";
            vec.remove(0);
            vec.add(value);
        }
        cout << "..." << endl;
    }
}

