// vampires.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Vampire declaration.

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    bool offTurn;
};

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    string dropPoisonVial();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     vampireCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfVampiresAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();

  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;

      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Vampire implementation
///////////////////////////////////////////////////////////////////////////

Vampire::Vampire(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A vampire must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Vampire created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_health = 2;
}

int Vampire::row() const
{
    return m_row;
}

int Vampire::col() const
{
    return m_col;
}

bool Vampire::isDead() const
{
    if (m_health == 0)
        return true;
    else
        return false;
}

void Vampire::move()
{
    int  dir =  randInt(0, NUMDIRS-1);
    
    if ((m_health == 1 && offTurn == false) || m_health == 2)
    {
        if (m_health == 1)
            offTurn = true;
        
        switch(dir) {
            case NORTH:
                if (m_row > 1)
                    m_row--;
                break;
            case SOUTH:
                if (m_row < m_arena->rows())
                    m_row++;
                break;
            case EAST:
                if (m_col < m_arena->cols())
                    m_col++;
                break;
            case WEST:
                if (m_col > 1)
                    m_col--;
                break;
            default:
                return;
        }
    }
    else
    {
        if (m_health == 1)
            offTurn = false;
        return;
    }
    
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
    {
        m_arena->setCellStatus(m_row, m_col, EMPTY);
        m_health--;
        if (m_health == 1)
            offTurn = true;
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonVial()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poisoned blood vial at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poisoned blood vial has been dropped.";
}

string Player::move(int dir)
{
    if (attemptMove(*m_arena, dir, m_row, m_col) == false)
        return "Player couldn't move; player stands.";
    
    for(int r = 0; r < m_arena->rows(); r++)
        for(int c = 0; c < m_arena->cols(); c++)
        {
            if (m_arena->numberOfVampiresAt(m_row, m_col) > 0)
            {
                setDead();
                return "Player walked into a vampire and died.";
            }
        }
    
    switch (dir)
    {
        case NORTH:
            return "Player moved north.";
            break;
        case SOUTH:
            return "Player moved south.";
            break;
        case EAST:
            return "Player moved east.";
            break;
        case WEST:
            return "Player moved west.";
            break;
    }
    
    return "";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nVampires = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    delete m_player;
    for (int  i = 0; i < m_nVampires;  i++)
        delete m_vampires[i];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::vampireCount() const
{
    return m_nVampires;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r-1][c-1];
}

int Arena::numberOfVampiresAt(int r, int c) const
{
    int nVampires = 0;
    
    for (int i = 0; i < vampireCount(); i++)
        if (m_vampires[i]->row() == r && m_vampires[i]->col() == c)
            nVampires++;
    
    return nVampires;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (poisoned blood vials)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

      // Indicate each vampire's position
      for(r = 0; r < rows(); r++)
          for(c = 0; c < cols(); c++)
              switch (numberOfVampiresAt(r+1,c+1))
              {
                  case 0: break;
                  case 1: displayGrid[r][c] = 'V';
                      break;
                  case 2: displayGrid[r][c] = '2';
                      break;
                  case 3: displayGrid[r][c] = '3';
                      break;
                  case 4: displayGrid[r][c] = '4';
                      break;
                  case 5: displayGrid[r][c] = '5';
                      break;
                  case 6: displayGrid[r][c] = '6';
                      break;
                  case 7: displayGrid[r][c] = '7';
                      break;
                  case 8: displayGrid[r][c] = '8';
                      break;
                  default: displayGrid[r][c] = '9';
                      break;
              }

      // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, vampire, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << vampireCount() << " vampires remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addVampire(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a vampire on a spot with a poisoned blood vial
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a vampire on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;

     // Don't add a vampire if there are MAXVAMPIRES existing vampires
    if (m_nVampires == MAXVAMPIRES)
        return false;

      // dynamically allocate a new vampire at coordinates (r,c).  Save the
      // pointer to newly allocated vampire and return true.
    
    m_vampires[m_nVampires] = new Vampire(this, r, c);
    m_nVampires++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a vampire
    if (numberOfVampiresAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveVampires()
{
    // Move all vampires
    for (int i = 0; i < m_nVampires; i++)
    {
        m_vampires[i]->move();
        if (m_vampires[i]->row() == m_player->row() && m_vampires[i]->col() ==  m_player->col())
            m_player->setDead();
    }
    
    for (int  i = 0; i < m_nVampires;  i++)
        if (m_vampires[i]->isDead())
        {
            delete m_vampires[i];
            for (int j = i; j < m_nVampires; j++)
            {
                m_vampires[j] = m_vampires[j+1];
            }
            i--;
            m_nVampires--;
        }
    
    
      // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nVampires)
{
    if (nVampires < 0)
    {
        cout << "***** Cannot create Game with negative number of vampires!" << endl;
        exit(1);
    }
    if (nVampires > MAXVAMPIRES)
    {
        cout << "***** Trying to create Game with " << nVampires
             << " vampires; only " << MAXVAMPIRES << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nVampires - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nVampires << " vampires!" << endl;
        exit(1);
    }

      // Create arena
    m_arena = new Arena(rows, cols);

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with vampires
    while (nVampires > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addVampire(r, c);
        nVampires--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonVial();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonVial();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->vampireCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        m_arena->moveVampires();
        m_arena->display(msg);
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    static random_device rd;
    static default_random_engine generator(rd());
    uniform_int_distribution<> distro(lowest, highest);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    switch(dir) {
        case NORTH:
            if (r > 1) {
                r--;
                return true;
            }
            break;
        case SOUTH:
            if (r < a.rows()) {
                r++;
                return true;
            }
            break;
        case EAST:
            if (c < a.cols()) {
                c++;
                return true;
            }
            break;
        case WEST:
            if (c > 1) {
                c--;
                return true;
            }
            break;
    }
    return false;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should drop a poisoned blood vial and
  // not move; otherwise, this function sets bestDir to the recommended
  // direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    int n = a.numberOfVampiresAt(r-1, c);
    int s = a.numberOfVampiresAt(r+1, c);
    int e = a.numberOfVampiresAt(r, c+1);
    int w = a.numberOfVampiresAt(r, c-1);
    
    int nn = a.numberOfVampiresAt(r-2, c);
    int nw = a.numberOfVampiresAt(r-1, c-1);
    int ne = a.numberOfVampiresAt(r-1, c+1);
    
    int ss = a.numberOfVampiresAt(r+2, c);
    int sw = a.numberOfVampiresAt(r+1, c-1);
    int se = a.numberOfVampiresAt(r+1, c+1);
    
    int ee = a.numberOfVampiresAt(r, c+2);
    int es = a.numberOfVampiresAt(r+1, c+1);
    int en = a.numberOfVampiresAt(r-1, c+1);
    
    int ww = a.numberOfVampiresAt(r, c-2);
    int ws = a.numberOfVampiresAt(r+1, c-1);
    int wn = a.numberOfVampiresAt(r-1, c-1);
    
    int moveN = n + nn + nw + ne;
    int moveS = s + ss + sw + se;
    int moveW = w + ww + ws + wn;
    int moveE = e + ee + es + en;
    
    if ((n + s + e + w) == 0 && a.getCellStatus(r,c) == 0)
        return false;
    
    if (moveN == 0 && r > 1) {
        bestDir = NORTH;
        return true;
    }
    else if (moveS == 0 && r < a.rows()) {
        bestDir = SOUTH;
        return true;
    }
    else if (moveE == 0 && c < a.cols()) {
        bestDir = EAST;
        return true;
    }
    else if (moveW == 0 && c > 1) {
        bestDir = WEST;
        return true;
    }
    
    if (moveN < moveS && moveN < moveE && moveN < moveW && r > 1) {
        bestDir = NORTH;
        return true;
    }
    else if (moveS < moveN && moveS < moveE && moveS < moveW && r < a.rows()) {
        bestDir = SOUTH;
        return true;
    }
    else if (moveE < moveS && moveE < moveN && moveE < moveW && c < a.cols()) {
        bestDir = EAST;
        return true;
    }
    else if (moveW < moveS && moveW < moveE && moveW < moveN && c > 1) {
        bestDir = WEST;
        return true;
    }
    else
        return false;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:
      // Game g(3, 5, 2);
    Game g(10, 12, 20);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                        &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif


