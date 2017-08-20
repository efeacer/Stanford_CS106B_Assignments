/**
 * Header file, defining Boggle object's public and private methods.
 * @author EFE ACER
 * @version 1.0
 * Section Leader: Ryan Kurohara
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "hashset.h"
using namespace std;

class Boggle {
public: //const methods are mainly accessors so that they do not change the state of the object.
    Boggle(Lexicon& dictionary, string boardText);
    char getLetter(int row, int col) const;
    bool checkWord(string word) const;
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman() const;
    int getScoreComputer() const;
    HashSet<string> getFoundWords() const;
    int getDimension() const;
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    bool humanWordSearch(string word, Grid<bool> & isUsed, int row, int col);
    void computerWordSearch(Set<string> & result, string word, Grid<bool> & isUsed, int row, int col);
    int dimension;
    int humanScore;
    int computerScore;
    Grid<char> gameBoard;
    Lexicon dictionary;
    HashSet<string> foundWords;
};

#endif // _boggle_h
