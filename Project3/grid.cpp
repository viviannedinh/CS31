#include "grid.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cctype>
using namespace std;

static char grid[MAXROWS][MAXCOLS];
static int nRows;
static int nCols;
static bool isGridInitialized = false;

static void drawHorizBorder();
static void checkPos(int r, int c, string func);

void setSize(int nr, int nc)
{
    if (nr < 1  ||  nr > MAXROWS)
    {
        cerr << "setSize: invalid number of rows " << nr << endl;
        exit(1);
    }
    if (nc < 1  ||  nc > MAXCOLS)
    {
        cerr << "setSize: invalid number of columns " << nc << endl;
        exit(1);
    }
    nRows = nr;
    nCols = nc;
    isGridInitialized = true;
    clearGrid();
}

int getRows()
{
    if (!isGridInitialized)
    {
        cerr << "getRows: You must first call setSize." << endl;
        exit(1);
    }
    return nRows;
}

int getCols()
{
    if (!isGridInitialized)
    {
        cerr << "getCols: You must first call setSize." << endl;
        exit(1);
    }
    return nCols;
}

void clearGrid()
{
    if (!isGridInitialized)
    {
        cerr << "clearGrid: You must first call setSize." << endl;
        exit(1);
    }
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
            grid[r][c] = ' ';
}

void setChar(int r, int c, char ch)
{
    checkPos(r, c, "setChar");
    grid[r-1][c-1] = ch;
}

char getChar(int r, int c)
{
    checkPos(r, c, "getChar");
    return grid[r-1][c-1];
}

void draw()
{
      // Write header lines of column numbers

    if (nCols >= 10)
    {
        cout << "   ";
        for (int c = 1; c <= nCols; c++)
        {
            int t = c / 10;
            if (t == 0)
                cout << ' ';
            else
                cout << t;
        }
        cout << endl;
    }
    cout << "   ";
    for (int c = 1; c <= nCols; c++)
        cout << (c % 10);
    cout << endl;

      // Draw grid

    drawHorizBorder();
    for (int r = 1; r <= nRows; r++)
    {
        cout << setw(2) << r << '|';
        for (int c = 1; c <= nCols; c++)
        {
            char ch = grid[r-1][c-1];
        if (isprint(ch))
            cout << ch;
            else
            cout << '?';
        }
        cout << '|' << endl;
    }
    drawHorizBorder();
}

static void drawHorizBorder()
{
    cout << "  +";
    for (int c = 1; c <= nCols; c++)
        cout << '-';
    cout << '+' << endl;
}

static void checkPos(int r, int c, string func)
{
    if (!isGridInitialized)
    {
        cout << func << ": You must first call setSize." << endl;
        exit(1);
    }
    if (r < 1  ||  r > nRows  ||  c < 1  ||  c > nCols)
    {
        cout << func << ": invalid position (" << r << "," << c << ")"
                 << endl;
        exit(1);
    }
}
