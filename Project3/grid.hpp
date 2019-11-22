// Grid library interface

#ifndef GRID_INCLUDED
#define GRID_INCLUDED

const int MAXROWS = 30;
const int MAXCOLS = 30;

void setSize(int nr, int nc);
int getRows();
int getCols();
void clearGrid();
void setChar(int r, int c, char ch);
char getChar(int r, int c);
void draw();

#endif // GRID_INCLUDED
