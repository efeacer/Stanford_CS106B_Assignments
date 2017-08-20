/**
 * Code involving functions to perform the interaction between the Boggle game and the user. Prints instructions,
 * reads inputs and so on.
 * @author EFE ACER
 * @version 1.0
 * Section Leader: Ryan Kurohara
 */

#include "lexicon.h"
#include "simpio.h"
#include "Boggle.h"
#include "bogglegui.h"
#include "console.h"
#include "strlib.h"

//constant declerations
const int SMALL_GUI_DIMENSION = 4;

//function declerations
void setUpGame(Boggle & game, string message);
void fillGameBoardGUI(Boggle &game, string & boardText);
void setUpGUI();
void playHumanTurn(Boggle & game);
void playComputerTurn(Boggle & game);
void announceWinner(Boggle & game);
string getBoardText();
bool isBoardTextValid(string & boardText);

/**
 * @brief playOneGame Performs one complete Boggle game, calls some helper functions.
 * @param dictionary The dictionary, which will determine valid words.
 */
void playOneGame(Lexicon& dictionary) { 
    setUpGUI();
    string boardText = getBoardText();
    Boggle game(dictionary, boardText);
    fillGameBoardGUI(game, boardText);
    playHumanTurn(game);
    playComputerTurn(game);
    announceWinner(game);
}

/**
 * @brief setUpGUI Initialisez the GUI for the game.
 */
void setUpGUI() {
    if (!BoggleGUI::isInitialized()) {
        BoggleGUI::initialize(SMALL_GUI_DIMENSION, SMALL_GUI_DIMENSION);
    } else {
        BoggleGUI::reset();         
    }
}

/**
 * @brief fillGameBoardGUI Fills the graphical game board accordingly.
 * @param game The reference to the Boggle object.
 * @param boardText The string determining how to fill the game board.
 */
void fillGameBoardGUI(Boggle & game, string & boardText) {
    if (boardText != "") {
        BoggleGUI::labelAllCubes(toUpperCase(boardText));
    } else {
        for (int r = 0; r < game.getDimension(); r++) {
            for (int c = 0; c < game.getDimension(); c++) {
                BoggleGUI::labelCube(r, c, stringToChar(toUpperCase(charToString(game.getLetter(r, c)))), false);
            }
        }
    }
}

/**
 * @brief setUpGame Sets the game up on the console.
 * @param game The reference to the Boggle object.
 * @param message A specified message to display on console.
 */
void setUpGame(Boggle & game, string message) {
    clearConsole();
    cout << message << endl << game << endl
         << "Your words (" << game.getFoundWords().size() << "): " << game.getFoundWords()
         << endl << "Your score: " << game.getScoreHuman() << endl;
}

/**
 * @brief playHumanTurn Plays the first half of the game, which is the players turn. Prints instructions, calls
 * Boggle functions and sets new graphics.
 * @param game The reference to the Boggle object.
 */
void playHumanTurn(Boggle & game) {
    clearConsole();
    setUpGame(game, "It's your turn!");
    BoggleGUI::setStatusMessage("It's your turn!");
    string word = toLowerCase(getLine("Type a word (or Enter to stop): "));
    while (word != "") {
        if (!game.checkWord(word)) {
            setUpGame(game, "You must enter an unfound 4+ letter word from the dictionary.");
            BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
        } else if (game.humanWordSearch(word)) {
            setUpGame(game, "You found a new word! \"" + toUpperCase(word) + "\"");
            BoggleGUI::setStatusMessage("You found a new word! \"" + toUpperCase(word) + "\"");
            BoggleGUI::recordWord(toLowerCase(word), BoggleGUI::HUMAN);
            BoggleGUI::setScore(game.getScoreHuman(), BoggleGUI::HUMAN);
        } else if (word != "") {
            setUpGame(game, "That word can't be formed on this board.");
            BoggleGUI::setStatusMessage("That word can't be formed on this board.");
        }
        word = toLowerCase(getLine("Type a word (or Enter to stop): "));
    }
}

/**
 * @brief playComputerTurn Plays the second half of the game, which is the computers turn. Prints instructions, calls
 * Boggle functions and sets new graphics.
 * @param game The reference to the Boggle object.
 */
void playComputerTurn(Boggle & game) {
    BoggleGUI::clearHighlighting();
    Set<string> wordsFoundByComputer = game.computerWordSearch();
    cout << "\nIt's my turn!\nMy words (" << wordsFoundByComputer.size() << "): " << wordsFoundByComputer
         << "\nMy score: " <<  game.getScoreComputer() << endl;
    BoggleGUI::setStatusMessage("It's my turn!");
    for (string word: wordsFoundByComputer) {
        BoggleGUI::recordWord(toLowerCase(word), BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(game.getScoreComputer(), BoggleGUI::COMPUTER);
}

/**
 * @brief announceWinner Announces the winner both in the console and the graphics window by comparing the scores of
 * the player and the computer.
 * @param game The reference to the Boggle object.
 */
void announceWinner(Boggle & game) {
    if (game.getScoreComputer() > game.getScoreHuman()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!\n";
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        cout << "WOW, you defeated me! Congratulations!\n";
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}

/**
 * @brief getBoardText Prompts and returns a string representing the letters on the game board.
 * @return A string representing the letters on the game board.
 */
string getBoardText() {
    if (getYesOrNo("Do you want to generate a random board?")) {
        return "";
    } else {
        string boardText;
        do {
            boardText = toLowerCase(getLine("Type the 16 letters to appear on the board: "));
            if (!isBoardTextValid(boardText)) {
                cout << "That is not a valid 16-letter board string. Try again.\n";
            }
        } while (!isBoardTextValid(boardText));
        return boardText;
    }
}

/**
 * @brief isBoardTextValid Checks whether the board text is valid or not, returns a bool.
 * @param boardText A string representing the letters on the game board.
 * @return True if the string is valid, false otherwise.
 */
bool isBoardTextValid(string & boardText) {
    if (boardText.length() == SMALL_GUI_DIMENSION * SMALL_GUI_DIMENSION) {
        for (char check: boardText) {
            if (!isalpha(check)) {
                return false;
            }
        }
        return true;
    }
    return false;
}
