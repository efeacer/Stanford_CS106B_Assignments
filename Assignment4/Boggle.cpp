/**
 * The Boggle class representing a Boggle game in which the objective of the player is to find the valid words on
 * the game board according to some set of rules. The code involves methods and properties to perform a Boggle game.
 * @author EFE ACER
 * @version 1.0
 * Section Leader: Ryan Kurohara
 */

#include "Boggle.h"
#include "shuffle.h"
#include "random.h"
#include "strlib.h"
#include "bogglegui.h"

//constant decleration(s)
const int ANIMATION_DELAY = 100; //constant to change animation delay for highlights

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/**
 * @brief Boggle::Boggle The constructor of the Boggle object. Initializes the dimension of the game board,
 * scores of the players, a few data structures to hold used words and a dictionary.
 * @param dictionary The reference of the Lexicon structure containing the dictionary words.
 * @param boardText The string representing the letters on the game board, the letters are randomized if
 * this string is passed empty.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dimension = 4;
    humanScore = 0;
    computerScore = 0;
    this->dictionary = dictionary;
    gameBoard.resize(dimension, dimension);
    if (boardText == "") {
        shuffle(CUBES, 16); //shuffling the cubes
        for (int r = 0; r < gameBoard.numRows(); r++) {
            for (int c = 0; c < gameBoard.numCols(); c++) {
                gameBoard[r][c] = stringToChar(toLowerCase(charToString(CUBES[r * dimension + c][randomInteger(0, 5)]))); //placing a random face of the cube
            }
        }
    } else {
        for (int r = 0; r < gameBoard.numRows(); r++) {
            for (int c = 0; c < gameBoard.numCols(); c++) {
                gameBoard[r][c] = boardText[r * dimension + c]; //placing the given boardText
            }
        }
    }
}

/**
 * @brief Boggle::getLetter Returns the letter in the specified row and column of the game board.
 * @param row The specified row of the game board.
 * @param col The specified column of the game board.
 * @return The letter in the specified position of the game board.
 */
char Boggle::getLetter(int row, int col) const {
    if (!gameBoard.inBounds(row,col)) {
        throw("Row and/or column are out of bounds.");
    }
    return gameBoard[row][col];
}

/**
 * @brief Boggle::checkWord Checks a word to be valid for the Boggle game. Returns true if the word is valid,
 * returns false otherwise.
 * @param word The word that will be checked for validity.
 * @return True if the word is valid, false otherwise.
 */
bool Boggle::checkWord(string word) const {
    if (dictionary.contains(toLowerCase(word)) && word.length() >= 4 && !foundWords.contains(toUpperCase(word))) {
        return true;
    }
    return false;
}

/**
 * @brief Boggle::humanWordSearch Helper recursive function for the humanWordSearch() method, uses backtracking
 * to find if there is a solution. The main purpose of the method is to search for a specified word in the game
 * board.
 * @param word The specified word that will be searched.
 * @param isUsed The boolean grid indicating the marked (visited) positions.
 * @param row The row of the game board, for the function to know where the search is happening.
 * @param col The column of the game board, for the function to know where the search is happening.
 * @return A boolean expression that is true if the word is found, false otherwise.
 */
bool Boggle::humanWordSearch(string word, Grid<bool> & isUsed, int row, int col) {
    BoggleGUI::setAnimationDelay(ANIMATION_DELAY); //highlighting
    BoggleGUI::setHighlighted(row, col, true);
    if (gameBoard[row][col] == word[0] && !isUsed[row][col]) {
        if (word.substr(1) == "") {
            return true;
        }
        isUsed[row][col] = true; //choose
        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if (gameBoard.inBounds(r, c) && !isUsed[r][c] && humanWordSearch(word.substr(1), isUsed, r, c)) { //explore
                    return true;
                }
            }
        }
        isUsed[row][col] = false; //un-choose
    }
    BoggleGUI::setAnimationDelay(ANIMATION_DELAY); //de-highlighting
    BoggleGUI::setHighlighted(row, col, false);
    return false;
}

/**
 * @brief Boggle::humanWordSearch Uses the humanWordSearch helper function of more parameters. Performs that backtracking
 * method on each cell of the game board. Searches for a specified word.
 * @param word The word to be searched.
 * @return A boolean expression that is true if the word is found, false otherwise.
 */
bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    Grid<bool> isUsed(dimension, dimension, false);
    if (checkWord(word)) {
        for (int r = 0; r < gameBoard.numRows(); r++) {
            for (int c = 0; c < gameBoard.numCols(); c++) {
                if (humanWordSearch(word, isUsed, r, c)) {
                    foundWords.add(toUpperCase(word));
                    humanScore += word.length() - 3;
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * @brief Boggle::getScoreHuman Accessor method for the score of the player.
 * @return The score of the player.
 */
int Boggle::getScoreHuman() const {
    return humanScore;
}

/**
 * @brief Boggle::computerWordSearch Helper method for computerWordSearch, uses recursive backtracking the find all
 * solutions to a problem. Finds and collects all possible valid words that can be found in the game board.
 * @param result The set containing all possible valid words that can be found in the game board.
 * @param word A string representing a possible valid word to be found.
 * @param isUsed The boolean grid indicating the marked (visited) positions.
 * @param row The row of the game board, for the function to know where the search is happening.
 * @param col The column of the game board, for the function to know where the search is happening.
 */
void Boggle::computerWordSearch(Set<string> & result, string word, Grid<bool> & isUsed, int row, int col) {
    if (checkWord(word)) {
        foundWords.add(toUpperCase(word));
        result.add(toUpperCase(word));
        computerScore += word.length() - 3;
    }
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            if (gameBoard.inBounds(r, c) && !isUsed[r][c] && dictionary.containsPrefix(word + gameBoard[r][c])) {
                isUsed[r][c] = true;
                computerWordSearch(result, word + gameBoard[r][c], isUsed, r, c);
                isUsed[r][c] = false;
            }
        }
    }
}

/**
 * @brief Boggle::computerWordSearch Uses computerWordSearch of more parameters. Performs that backtracking method
 * on each cell of the game board to find all possible words that can be found on the game board.
 * @return The Set of words containing all possible words generated from the game board.
 */
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    Grid<bool> isUsed(dimension, dimension, false);
    for (int r = 0; r < gameBoard.numRows(); r++) {
        for (int c = 0; c < gameBoard.numCols(); c++) {
            computerWordSearch(result, "", isUsed, r, c);
        }
    }
    return result;
}

/**
 * @brief Boggle::getScoreComputer Accessor method for the score of the computer.
 * @return The score of the computer.
 */
int Boggle::getScoreComputer() const {
    return computerScore;
}

/**
 * @brief Boggle::getFoundWords Accessor method for the HashSet of found words.
 * @return The HashSet of found words.
 */
HashSet<string> Boggle::getFoundWords() const {
    return foundWords;
}

/**
 * @brief Boggle::getDimension Accessor method for the dimension of the game board.
 * @return The dimension of the game board.
 */
int Boggle::getDimension() const {
    return dimension;
}

/**
 * @brief operator << The operator overloaded to print a Boggle object.
 * @param out The ostream.
 * @param boggle A reference to the Boggle object.
 * @return The ostream with Boggle objects string representation.
 */
ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int r = 0; r < boggle.gameBoard.numRows(); r++) {
        for (int c = 0; c < boggle.gameBoard.numCols(); c++) {
            out << toUpperCase(charToString(boggle.getLetter(r,c)));
        }
        out << "\n";
    }
    return out;
}
