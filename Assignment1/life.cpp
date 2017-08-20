/**
  * This program is a console based simulation of "The Game of Life", which is indeed
  * a simulation for modelling the life cycle of bacteria using a two-dimensional grid
  * of cells. The game simulates the birth and death of future generations based on
  * an initial pattern and some simple rules. The code involves variables and fuctions
  * to model the game.
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
const string FILE_ERROR = "Unable to open that file.  Try again.\n";
const string OPTIONS = "Should the simulation wrap around the grid (y/n)? ";
const string MENU = "a)nimate, t)ick, q)uit? ";
const string PROMPT_FRAME_NUMBER = "How many frames? ";
const string ERROR = "Invalid choice; please try again.\n";
const int PAUSE = 50;

//Function declerations
void displayGrid(Grid<string> &grid);
void advanceGrid(Grid<string> &grid, bool wrapping);
int countNeighbours(Grid<string> &grid, int row, int col, bool wrapping);

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
    int frameNo;

    //Displaying the intro welcome message
    cout << WELCOME_MESSAGE;

    //Prompting a file and processing it
    do {
        file = getLine(PROMPT_FILE);
        if (!isFile(file)) {
            cout << FILE_ERROR;
        }
    } while (!isFile(file));
    openFile(stream, file);
    //Constructing the grid, using the information in the file
    getline(stream, row);
    getline(stream, col);
    Grid<string> grid(stringToInteger(row), stringToInteger(col));
    //filling the grid accordingly
    for (int r = 0; r < grid.numRows(); r++) {
        getline(stream, toPut);
        for (int c = 0; c < grid.numCols(); c++) {
            grid[r][c] = toPut[c];
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
    displayGrid(grid);
    do {
        choice = getLine(MENU);
        if (equalsIgnoreCase(choice, "a")) {
            //animating the pattern
            frameNo = getInteger(PROMPT_FRAME_NUMBER);
            for (int i = 0; i < frameNo; i++) {
                clearConsole();
                advanceGrid(grid, wrapAround);
                pause(PAUSE);
            }
        }
        else if (equalsIgnoreCase(choice, "t")) {
            advanceGrid(grid, wrapAround);
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
 * @brief displayGrid Prints the parametrized grid to the console.
 * @param grid The grid that will be printed.
 */
void displayGrid(Grid<string> &grid) {
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            cout << grid[r][c];
        }
        cout << endl;
    }
}

/**
 * @brief advanceGrid Advances the grid to the next generation based on a bunch of rules.
 * @param grid The grid that will be advanced.
 * @param wrapping A bool type expression indicating whether the grid is wrapping around
 * itself or not.
 */
void advanceGrid(Grid<string> &grid, bool wrapping) {
    Grid<int> counts(grid.numRows(), grid.numCols());
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            counts[r][c] = countNeighbours(grid, r, c, wrapping);
        }
    }
    for (int r = 0; r < grid.numRows(); r++) {
        for (int c = 0; c < grid.numCols(); c++) {
            if (counts[r][c] <= 1 || counts[r][c] >= 4) {
                grid.set(r, c, "-");
            }
            else if (counts[r][c] == 3) {
                grid.set(r, c, "X");
            }
        }
    }
    displayGrid(grid);
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



