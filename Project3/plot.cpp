#include "grid.hpp"
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

//declare and initialize constant variables
const int HORIZ = 0;
const int VERT = 1;
const int FG = 0;
const int BG = 1;

//prototype functions
bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg);
void plotHorizontalLine(int r, int c, int distance, char ch, int fgbg);
void plotVerticalLine(int r, int c, int distance, char ch, int fgbg);
int performCommands(string commandString, char& plotChar, int& mode, int& badPos);

int main()
{
    // set grid and default mode and char
    setSize(20, 30);
    char currentChar = '*';
    int currentMode = FG;
    
    // repeatedly prompt user for command string
    for (;;)
    {
        // get user input for command string, terminate if empty string
        cout << "Enter a command string: ";
        string cmd;
        getline(cin, cmd);
        if (cmd == "")
            break;

        // display error or display grid w plots
        int position;
        int status = performCommands(cmd, currentChar, currentMode, position);
        switch (status)
        {
            case 0:
                draw();
                break;
            case 1:
                cout << "Syntax error at position " << position+1 << endl;
                break;
            case 2:
                cout << "Cannot perform command at position " << position+1 << endl;
                break;
            default:
                // It should be impossible to get here.
                cerr << "performCommands returned " << status << "!" << endl;
        }
    }
}

int performCommands(string commandString, char& plotChar, int& mode, int& badPos)
{
    //declare and initialize variables for plotting
    int c = 1;
    int r = 1;
    string temp = "";
    int distance;
    int dir;
    int i = 0;
    
    // read command string
    while(i < commandString.size())
    {
        // plotting command is 'c' or 'C'
        if (tolower(commandString[i]) == 'c')
        {
            // clear grid and reset values to default
            clearGrid();
            plotChar = '*';
            r = 1;
            c = 1;
            mode = FG;
            i++;
        }
        // plotting command is 'v' or 'V'
        else if (tolower(commandString[i]) == 'v')
        {
            dir = VERT;
            if (commandString[i+1] == '-')
            {
                i++;
                // number after command is negative and double digit
                if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
                {
                    temp = commandString[i+1];
                    temp += commandString[i+2];
                    stringstream(temp) >> distance;
                    distance = distance * -1;
                    if (plotLine(r,c,distance,dir,plotChar,mode))
                    {
                        i += 3;
                        r += distance;
                    }
                    else
                    {
                        badPos = i-1;
                        return 2;
                    }
                }
                // number after command is negative and single digit
                else if (isdigit(commandString[i+1]))
                {
                    temp = commandString[i+1];
                    stringstream(temp) >> distance;
                    distance = distance * -1;
                    if (plotLine(r,c,distance,dir,plotChar,mode))
                    {
                        i += 2;
                        r += distance;
                    }
                    else
                    {
                        badPos = i-1;
                        return 2;
                    }
                }
                else
                {
                    badPos = i;
                    return 1;
                }
            }
            // number after command is positive and double digit
            else if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
            {
                temp = commandString[i+1];
                temp += commandString[i+2];
                stringstream(temp) >> distance;
                if (plotLine(r,c,distance,dir,plotChar,mode))
                {
                    i += 3;
                    r += distance;
                }
                else
                {
                    badPos = i;
                    return 2;
                }
            }
            // number after command is positive and single digit
            else if (isdigit(commandString[i+1]))
            {
                temp = commandString[i+1];
                stringstream(temp) >> distance;
                if (plotLine(r,c,distance,dir,plotChar,mode))
                {
                    i += 2;
                    r += distance;
                }
                else
                {
                    badPos = i;
                    return 2;
                }
            }
            else
            {
                badPos = i+1;
                return 1;
            }
        }
        // plotting command is 'h' or 'H'
        else if (tolower(commandString[i]) == 'h')
        {
            dir = HORIZ;
            if (commandString[i+1] == '-')
            {
                i++;
                // number after command is negative and double digit
                if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
                {
                    temp = commandString[i+1];
                    temp += commandString[i+2];
                    stringstream(temp) >> distance;
                    distance = distance * -1;
                    if (plotLine(r,c,distance,dir,plotChar,mode))
                    {
                        i += 2;
                        c += distance;
                    }
                    else
                    {
                        badPos = i-1;
                        return 2;
                    }
                }
                // number after command is negative and single digit
                else if (isdigit(commandString[i+1]))
                {
                    temp = commandString[i+1];
                    stringstream(temp) >> distance;
                    distance = distance * -1;
                    if (plotLine(r,c,distance,dir,plotChar,mode))
                    {
                        i += 2;
                        c += distance;
                    }
                    else
                    {
                        badPos = i-1;
                        return 2;
                    }
                }
                else
                {
                    badPos = i;
                    return 1;
                }
            }
            // number after command is positive and double digit
            else if (isdigit(commandString[i+1]) && isdigit(commandString[i+2]))
            {
                temp = commandString[i+1];
                temp += commandString[i+2];
                stringstream(temp) >> distance;
                if (plotLine(r,c,distance,dir,plotChar,mode))
                {
                    i += 3;
                    c += distance;
                }
                else
                {
                    badPos = i;
                    return 2;
                }
            }
            // number after command is positive and single digit
            else if (isdigit(commandString[i+1]))
            {
                temp = commandString[i+1];
                stringstream(temp) >> distance;
                if (plotLine(r,c,distance,dir,plotChar,mode))
                {
                    i += 2;
                    c += distance;
                }
                else
                {
                    badPos = i;
                    return 2;
                }
            }
            // character after plotting command is not valid
            else
            {
                badPos = i+1;
                return 1;
            }
            
        }
        // plotting command is 'f' or 'F'
        else if (tolower(commandString[i]) == 'f')
        {
            if (isprint(commandString[i+1]))
            {
                // change mode and char
                plotChar = commandString[i+1];
                mode = FG;
                i += 2;
            }
            else
            {
                badPos = i+1;
                return 1;
            }
        }
        // plotting command is 'b' or 'B'
        else if (tolower(commandString[i]) == 'b')
        {
            if (isprint(commandString[i+1]))
            {
                // change mode and char
                plotChar = commandString[i+1];
                mode = BG;
                i += 2;
            }
            else
            {
                badPos = i+1;
                return 1;
            }
        }
        // command is not a valid plotting command (v,h,b,f,c)
        else
        {
            badPos = i+1;
            i++;
            return 1;
        }
    }
    // returns 0 when no syntax errors
    return 0;
}

bool plotLine(int r, int c, int distance, int dir, char plotChar, int fgbg)
{
    // check for correctness of values passed to function
    if (dir != HORIZ && dir != VERT)
        return false;
    else if (fgbg != FG && fgbg != BG)
        return false;
    else if (! isprint(plotChar))
        return false;
    else if (r < 1 || r > getRows() || c < 1 || c > getCols())
        return false;
    else if (dir == 1 && (r + distance < 1 || r + distance > getRows()))
        return false;
    else if (dir == 0 && (c + distance < 1 || c + distance > getCols()))
        return false;
    else
    {
        // calls plotting functions if all values valid
        if (dir == HORIZ)
            plotHorizontalLine(r,c,distance,plotChar,fgbg);
        else if (dir == VERT)
            plotVerticalLine(r,c,distance,plotChar,fgbg);
        return true;
    }
}

void plotHorizontalLine(int r, int c, int distance, char ch, int fgbg)
{
    // plot leftward
    if (distance < 0)
    {
        for (int i = abs(distance); i >= 0; i--, c--)
            if (fgbg == 0 || (fgbg == 1 && getChar(r,c) == ' '))
                setChar(r, c, ch);
        c++;
    }
    // plot rightward
    else if (distance >= 0)
    {
        for (int i = 0; i <= distance && c <= getCols(); i++, c++)
            if (fgbg == 0 || (fgbg == 1 && getChar(r,c) == ' '))
                setChar(r, c, ch);
        c--;
    }
}

void plotVerticalLine(int r, int c, int distance, char ch, int fgbg)
{
    // plot up
    if (distance < 0)
    {
        for (int i = abs(distance); i >= 0; i--, r--)
            if (fgbg == 0 || (fgbg == 1 && getChar(r,c) == ' '))
                setChar(r, c, ch);
        r++;
    }
    //plot down
    else if (distance >= 0)
    {
        for (int i = 0; i <= distance && r <= getRows(); i++, r++)
            if (fgbg == 0 || (fgbg == 1 && getChar(r,c) == ' '))
                setChar(r, c, ch);
        r--;
    }
}
