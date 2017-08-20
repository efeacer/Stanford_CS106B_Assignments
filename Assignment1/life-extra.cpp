/**
  * LIFE - EXTRA (CONTAINS EXTENSIONS)
  * This program is a console based simulation of "The Game of Life", which is indeed
  * a simulation for modelling the life cycle of bacteria using a two-dimensional grid
  * of cells. The game simulates the birth and death of future generations based on
  * an initial pattern and some simple rules. The code involves variables and fuctions
  * to model the game. The program also contains some extensions. Those extensions involve
  * random pattern generation and GUI. Random pattern generation code can be found in the
  * function named generateRandomGrid and GUI codes are added inside displayGrid, advanceGrid
  * and main functions.
  * @author EFE ACER
  * CS106B - Section Leader: Ryan Kurohara
  */

//necessary includes
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
#include "random.h" //added it for the extensions
using namespace std;

//Constant declerations (for further changes)
const string WELCOME_MESSAGE = "Welcome to the CS 106B Game of Life,\n"
                               "a simulation of the lifecycle of a bacteria colony.\n"
                               "Cells (X) live and die by the following rules:\n"
                               "- A cell with 1 or fewer neighbors dies.\n"
                               "- Locations with 2 neighbors remain stable.\n"
                               "- Locations with 3 neighbors will create life.\n"
                               "- A cell with 4 or more neighbors dies.\n\n";
const string PROMPT_FILE = "Grid input file name? ";
const string RANDOM = "(type \"random\" to generate a random pattern) ";
const string FILE_ERROR = "Unable to open that file.  Try again.\n";
const string OPTIONS = "Should the simulation wrap around the grid (y/n)? ";
const string MENU = "a)nimate, t)ick, q)uit? ";
const string PROMPT_FRAME_NUMBER = "How many frames? ";
const string ERROR = "Invalid choice; please try again.\n";
const int PAUSE = 50;

//Function declerations
void displayGrid(Grid<string> &grid, LifeGUI &GUIgrid);
void advanceGrid(Grid<string> &grid, bool wrapping, LifeGUI &GUIgrid);
int countNeighbours(Grid<string> &grid, int row, int col, bool wrapping);
void generateRandomGrid(Grid<string> &grid, LifeGUI &GUIgrid);

//main function of the program
int main() {
    //Variables
    string file;
    ifstream stream;
    string row;
    string col;
    string toPut;
    string wrap;
    string choice;
    bool wrapAround;
    bool random;
    int frameNo;

    //Displaying the intro welcome message
    cout << WELCOME_MESSAGE;

    //Prompting a file and processing it
    random = false;
    do {
        file = getLine(PROMPT_FILE + RANDOM);
        if (file == "random") { //additional code for random world generation
            random = true;
        }
        else if (!isFile(file)) {
            cout << FILE_ERROR;
        }
    } while (!isFile(file) && file != "random");
    LifeGUI GUIgrid; //Grid<string> grid;
    Grid<string> grid;
    if (random) {
        generateRandomGrid(grid, GUIgrid);
    }
    else {
        openFile(stream, file);
        //Constructing the grid, using the information in the file
        getline(stream, row);
        getline(stream, col);
        GUIgrid.resize(stringToInteger(row), stringToInteger(col));
        grid.resize(stringToInteger(row), stringToInteger(col));
        //filling the grid accordingly
        for (int r = 0; r < grid.numRows(); r++) {
            getline(stream, toPut);
            for (int c = 0; c < grid.numCols(); c++) {
                grid[r][c] = toPut[c];
            }
        }
    }
    //Updating the grid and the menu options
    do {
        wrap = getLine(OPTIONS);
        if (!equalsIgnoreCase(wrap, "y") && !equalsIgnoreCase(wrap, "n")) {
            cout << ERROR;
        }
    } while (!equalsIgnoreCase(wrap, "y") && !equalsIgnoreCase(wrap, "n"));
    if (equalsIgnoreCase(wrap, "y")) {
        wrapAround = true;
    }
    else if (equalsIgnoreCase(wrap, "n")) {
        wrapAround = false;
    }
    displayGrid(grid, GUIgrid);
    do {
        choice = getLine(MENU);
        if (equalsIgnoreCase(choice, "a")) {
            //animating the pattern
            frameNo = getInteger(PROMPT_FRAME_NUMBER);
            for (int i = 0; i < frameNo; i++) {
                clearConsole();
                advanceGrid(grid, wrapAround, GUIgrid);
                pause(PAUSE);
            }
        }
        else if (equalsIgnoreCase(choice, "t")) {
            advanceGrid(grid, wrapAround, GUIgrid);
        }
        else if (equalsIgnoreCase(choice, "q")) {}
        else {
            cout << ERROR;
        }
    } while (!equalsIgnoreCase(choice, "q"));

    //ending message
    cout << "Have a nice Life!" << endl;
    return 0;
}

/**
 * @brief displayGrid Prints the parametrized grid to the console, also displays the GUI
 * representation of the grid.
 * @param grid The grid that will be printed.
 * @param GUIgrid The GUI reference of the grid, which will be displayed.
 */
void displayGrid(Grid<string> &grid, LifeGUI &GUIgrid) {
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            cout << grid[r][c];
            if (grid[r][c] == "X") {
                GUIgrid.drawCell(r, c, true);
            }
            else if (grid[r][c] == "-") {
                GUIgrid.drawCell(r, c, false);
            }
        }
        cout << endl;
    }
}

/**
 * @brief advanceGrid Advances the grid to the next generation based on a bunch of rules, prints it
 * to the console and displays it with a GUI.
 * @param grid The grid that will be advanced.
 * @param wrapping A bool type expression indicating whether the grid is wrapping around
 * itself or not.
 * @param GUIgrid The GUI reference of the grid, which will be updated to its' next generation.
 */
void advanceGrid(Grid<string> &grid, bool wrapping, LifeGUI &GUIgrid) {
    Grid<int> counts(grid.numRows(), grid.numCols());
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            counts[r][c] = countNeighbours(grid, r, c, wrapping);
        }
    }
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            if (counts[r][c] <= 1 || counts[r][c] >= 4) {
                GUIgrid.drawCell(r, c, false);
                grid[r][c] = "-";
            }
            else if (counts[r][c] == 3) {
                GUIgrid.drawCell(r, c, true);
                grid[r][c] = "X";
            }
        }
    }
    displayGrid(grid, GUIgrid);
}

/**
 * @brief countNeighbours Counts the neighbour cells of a specified one.
 * @param grid The grid that contains the cells.
 * @param row The row number of the specified cell.
 * @param col The column number of the specified cell.
 * @param wrapping A bool type expression indicating whether the grid is wrapping around
 * itself or not.
 * @return The number of neighbours of the specified cell.
 */
int countNeighbours(Grid<string> &grid, int row, int col, bool wrapping) {
    int count = 0;
    if (!wrapping) {
        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if (row != r || col != c) {
                    if (grid.inBounds(r, c)) {
                        if (grid[r][c] == "X") {
                            count++;
                        }
                    }
                }
            }
        }
    }
    else {
        int numRows = grid.numRows();
        int numCols = grid.numCols();
        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if (row != r || col != c) {
                    if (grid.inBounds(r, c)) {
                        if (grid[r][c] == "X") {
                            count++;
                        }
                    }
                    else {
                        if ((r == -1 || r == numRows) && (c == -1 || c == numCols)) {
                            if (grid[(r + numRows) % numRows][(c + numCols) % numCols] == "X") {
                                count++;
                            }

                        }
                        else if (r == -1 || r == numRows) {
                            if (grid[(r + numRows) % numRows][c] == "X") {
                                count++;
                            }
                        }
                        else if (c == -1 || c == numCols) {
                            if (grid[r][(c + numCols) % numCols] == "X") {
                                count++;
                            }
                        }
                    }
                }
            }
        }
    }
    return count;
}

/**
 * @brief generateRandomGrid The function generates a randomly sized and randomly filled grid of
 * dead and living cells. This one is a part of extensions.
 * @param grid The referenced grid that will be randomly sized and filled.
 * @param GUIgrid The GUI reference of the grid that will be updated.
 */
void generateRandomGrid(Grid<string> &grid, LifeGUI &GUIgrid) {
    int randomRow = randomInteger(1, 50);
    int randomCol = randomInteger(1, 50);
    GUIgrid.resize(randomRow, randomCol);
    grid.resize(randomRow, randomCol);
    int possibility = randomInteger(1, 20); //random possibility
    //placing the living cells to the grid
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            if (randomInteger(1, 20) <= possibility) { //placing the living cells depending on the random
                GUIgrid.drawCell(r, c, true);               //possibility
                grid[r][c] = "X";
            }
            else {
                GUIgrid.drawCell(r, c, false);
                grid[r][c] = "-";
            }
        }
    }
}


