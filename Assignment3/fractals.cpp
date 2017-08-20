/*
 * EXTENSION: Did the optional recursive tree part as an extension.
 * File: fractals.cpp
 * --------------------------
 * Name: EFE ACER
 * Section leader: Ryan Kurohara
 * This file contains fractal problems for CS106B.
 */


#include "fractals.h"
#include <cmath>

using namespace std;

const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */

/**
 * @brief drawEquilateralTriangle Draws a tip-down equilateral triangle on a graphics window
 * with respect to the specified size and coordinates.
 * @param gw The graphics window, where to draw the equilateral triangle.
 * @param x The x coordinate of the top-left corner of the equilateral triangle.
 * @param y The y coordinate of the top-left corner of the equilateral triangle.
 * @param size The length of a side of the equilateral triangle.
 */
void drawEquilateralTriangle(GWindow & gw, double & x, double & y, double & size) {
    gw.drawLine(x, y, x + size, y);
    gw.drawLine(x, y, x + size / 2, y + size * sqrt(3) / 2);
    gw.drawLine(x + size, y, x + size / 2, y + size * sqrt(3) / 2);
}

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow & gw, double x, double y, double size, int order) {
    if (x < 0 || y < 0 || size < 0 || order < 0) { //causes error
        throw("invalid input");
    } else if (order != 0) {
        if (order == 1) { //base case
            drawEquilateralTriangle(gw, x, y, size); //drawing the triangle for order 1
        } else { //recursive step
            drawSierpinskiTriangle(gw, x, y, size / 2, order - 1);
            drawSierpinskiTriangle(gw, x + size / 2, y, size / 2, order - 1);
            drawSierpinskiTriangle(gw, x + size / 4, y + size * sqrt(3) / 4, size / 2, order - 1);
        }
    }
}

/**
 * @brief drawTree Draws a recursive tree fractal with the specified order and size.
 * The top-left corner of the bounding box is at position (x,y).
 * @param gw The graphics window, where to draw the recursive tree image.
 * @param x The x coordinate of the top-left corner of the bounding box.
 * @param y The y coordinate of the top-left corner of the bounding box.
 * @param size The length of one side of the bounding box.
 * @param order The order of the fractal.
 * @param angle The angle indicating the direction of the branches.
 */
void drawTree(GWindow & gw, double x, double y, double size, int order, double angle) {
    if (x < size / 2 || y < size || size < 0 || order < 0) { //causes error
        throw("invalid input");
    } else if (order != 0) { //recursive step (base case is when order = 0 (draw nothing)
        if (order >= 2) { //setting the color of the branch
            gw.setColor(BRANCH_COLOR);
        } else { //setting the colot of the leaves
            gw.setColor(LEAF_COLOR);
        }
        gw.drawPolarLine(x, y, size / 2, 180 * angle / M_PI); //drawing a directed branch
        for (double i = -M_PI / 4; i <= M_PI / 4; i += M_PI / 12) { //recursing in different directions
            drawTree(gw, x + cos(angle) * size / 2, y - sin(angle) * size / 2, size / 2, order - 1, angle + i);
        }
    }
}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    drawTree(gw, x + size / 2, y + size, size, order, M_PI / 2);
}

/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    // To use palette:
    // pixels[r][c] = palette[numIterations % palette.size()];
    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    for (int r = 0; r < pixels.numRows(); r++) { //painting the image
        for (int c = 0; c < pixels.numCols(); c++) {
            int numIterations = mandelbrotSetIterations(Complex(minX + c * incX, minY + r * incY), 200);
            if (color != 0) {
                if (numIterations == maxIterations) {
                    pixels[r][c] = color;
                }
            } else {
                pixels[r][c] = palette[numIterations % palette.size()];
            }
        }
    }

    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex c, int maxIterations) {
    return mandelbrotSetIterations(Complex(0, 0), c, maxIterations);
}

/**
 * @brief mandelbrotSetIterations Determines whether a number is a member of the mandelbrot
 * set or not.
 * @param z Complex number for a given number of iterations.
 * @param c Complex number to use for recursive formula.
 * @param remainingIterations The remaining number of recursive runs.
 * @param maxIterations The initial number of remaining iterations.
 * @return Number of iterations needed to determine if c is unbounded.
 */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations, int & maxIterations) {
    if ((z * z + c).abs() >= 4 || remainingIterations == 0) { //base case (the value diverges or c is succesfully
        return maxIterations - remainingIterations;           // determined as a mandelbrot set number)
    } else { //recursive step (passing through the next recursively defined stage of the function)
        return mandelbrotSetIterations(z * z + c, c, remainingIterations - 1, maxIterations);
    }
}

/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    return mandelbrotSetIterations(z, c, remainingIterations, remainingIterations);
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
