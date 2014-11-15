// robots.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cassert>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the arena
const int MAXCOLS = 20;             // max number of columns in the arena
const int MAXROBOTS = 100;          // max number of robots allowed
const int MAXCHANNELS = 3;          // max number of channels
const double WALL_DENSITY = 0.11;   // density of walls

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int WALL  = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Robot declaration.

class Robot
{
  public:
        // Constructor
    Robot(Arena* ap, int r, int c, int channel);

        // Accessors
    int  row() const;
    int  col() const;
    int  channel() const;
    bool isDead() const;

        // Mutators
    void forceMove(int dir);
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_channel;
	int    m_damage;
};

class Player
{
  public:
        // Constructor
    Player(Arena *ap, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

        // Mutators
    string stand();
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
    int     robotCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRobotsAt(int r, int c) const;
    void    display(string msg) const;

        // Mutators
    void   setCellStatus(int r, int c, int status);
    bool   addRobot(int r, int c, int channel);
    bool   addPlayer(int r, int c);
    string moveRobots(int channel, int dir);

  private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;

        // Helper functions
    void checkPos(int r, int c) const;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();

        // Mutators
    void play();

  private:
    Arena* m_arena;

        // Helper functions
    string takePlayerTurn();
    string takeRobotsTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool charToDir(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);

int numRobotsAround(const Arena& a, int r, int c);
bool outOfGrid(const Arena& a, int r, int c);
bool inWall(const Arena& a, int r, int c);

void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Robot implementation
///////////////////////////////////////////////////////////////////////////

Robot::Robot(Arena* ap, int r, int c, int channel)
{
    if (ap == NULL)
    {
        cout << "***** A robot must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Robot created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    if (channel < 1  ||  channel > MAXCHANNELS)
    {
        cout << "***** Robot created with invalid channel " << channel << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_channel = channel;
	m_damage=0; //initialize the robot's damage to zero (no damage taken yet)
}

int Robot::row() const
{
    return m_row;
}

int Robot::col() const
{
    return m_col;
}

int Robot::channel() const
{
    return m_channel;
}

bool Robot::isDead() const
{
	return m_damage>=3; //the amount of damage (at least 3) tells if robot is dead
}

void Robot::forceMove(int dir)
{
	if(!isDead()) //only functional robots can move
	{
		bool moved = attemptMove(*m_arena, dir, m_row, m_col);
		//attempt to move in a specified direction; if we can't move, don't move

		if(!moved) //if robot did not move due to grid restrictions or a wall
			m_damage++; //increment the damage of the robot
	}
}

void Robot::move()
{
    if (!isDead()) //only functional robots can move
        attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col);
		//attempt to move in a random direction; if we can't move, don't move (no damage taken)
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == NULL)
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

string Player::stand()
{
    return "Player stands.";
}

string Player::move(int dir)
{
	//attempt to move in a specified direction; if we can't move, don't move
	bool moved = attemptMove(*m_arena, dir, m_row, m_col);

	if(!moved) //player did not move due to grid restrictions or a wall
		return "Player couldn't move; player stands.";
	else if(m_arena->numberOfRobotsAt(m_row, m_col)>0) //if player moves into a robot
	{
		this->setDead(); //set player to dead state
		return "Player walked into a robot and died.";
	}
	else
	{
		switch(dir) //player moved, so print out corresponding diretion
		{
			case 0:
				return "Player moved north.";
				break;
			case 1:
				return "Player moved east.";
				break;
			case 2:
				return "Player moved south.";
				break;
			case 3:
				return "Player moved west.";
				break;
			default:
				return ""; //this case should never happen
				break;
		}
	}
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
    m_player = NULL;
    m_nRobots = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	delete m_player; //return the player's dynamically-allocated memory
	
	for(int i=0; i<m_nRobots; i++)
	{
		if(m_robots[i]!=NULL)
			delete m_robots[i]; //delete all robots that are not NULL (have not been previously deleted)
	}

	//delete [] m_robots; //some previously-released robots might be re-deleted, causing a crash; use for-loop instead
	
	
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

int Arena::robotCount() const
{
	return m_nRobots;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfRobotsAt(int r, int c) const
{
	int count=0; //number for counting the amount of robots at that spot

	for(int i=0; i<m_nRobots; i++)
	{
		if(m_robots[i]==NULL) //just in case: skip current iteration if that robot is NULL
			continue;

		Robot* robotPtr = m_robots[i]; //temporary robot pointer

		if((*robotPtr).row()==r && (*robotPtr).col()==c)
			count++; //for each robot, increment count if rol and col match those in parameters
	}

	return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (wall)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

      // Indicate robot positions by their channels.  If more than one robot
      // occupies a cell, show just one (any one will do).
	for(int i=0; i<m_nRobots; i++)
	{
		if(m_robots[i]==NULL) //just in case: skip current iteration if that robot is NULL
			continue;

		Robot* robotPtr = m_robots[i];
		char channel = '0' + robotPtr->channel(); //determine char counterpart of each robot's channel

		displayGrid[robotPtr->row()-1][robotPtr->col()-1] = channel; //input robot channel to grid
	}
	
      // Indicate player's position
    if (m_player != NULL)
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

      // Write message, robot, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == NULL)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;




	/*
	//TEST PURPOSES ONLY
	for(int i=0; i<10; i++)
	{
		if(m_robots[i]!=NULL)
			cerr << m_robots[i]->channel() << ": " << m_robots[i]->row() << ", " << m_robots[i]->col() << "; dead? " << m_robots[i]->isDead() << endl;
		else
			cerr << "This one is NULL" << endl;
	}
	*/
	
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addRobot(int r, int c, int channel)
{
    if (m_nRobots == MAXROBOTS)
         return false;
    m_robots[m_nRobots] = new Robot(this, r, c, channel);
    m_nRobots++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (m_player != NULL)
        return false;
    m_player = new Player(this, r, c);
    return true;
}

string Arena::moveRobots(int channel, int dir)
{
      // Robots on the channel will respond with probability 1/2
    
	bool willRespond = (randInt(0, 1) == 0); //provides boolean with probability of 1/2
	
      // Move all robots
    int nRobotsOriginally = m_nRobots; //record number of robots to start with prior to destroying them

	for(int i=0; i<m_nRobots; i++)
	{
		if(m_robots[i]==NULL) //just in case: skip current iteration if that robot is NULL
			continue;

		if(willRespond)
		{
			if(m_robots[i]->channel()==channel)
				m_robots[i]->forceMove(dir); //if controller works, force-move robots on channel
			else
				m_robots[i]->move(); //other robots not on channel move randomly
		}
		else
			m_robots[i]->move(); //if controller does not work, all robots move randomly
	}

	Player* playerPtr = this->player(); //determine row and col of the player
	int r = (*playerPtr).row();
	int c = (*playerPtr).col();

	if(this->numberOfRobotsAt(r, c)>0) //at least one robot is stomping on the player
		(*playerPtr).setDead(); //set the player to dead state
		
	for(int i=0; i<nRobotsOriginally; i++)
	{
		if(m_robots[i]==NULL) //just in case: skip current iteration if that robot is NULL
			continue;

		if((*m_robots[i]).isDead()) //if a robot is found to be dead
		{
			m_nRobots--; //decrement the number of robots
			delete m_robots[i]; //return that dynamically-allocated memory to computer
			m_robots[i]=NULL; //set robot to NULL so other methods do not cause undefined behavior
		}
	}

	if(m_nRobots<nRobotsOriginally) //if any robots destroyed, sort the array by putting NULLs in back
		for(int i=0; i<m_nRobots; i++) //check elements in the in-use-array
				for(int k=m_nRobots; k<nRobotsOriginally; k++) //check elements in the non-used-array
					if(m_robots[i]==NULL && m_robots[k]!=NULL)
					{
						//swap the NULLs in the in-use-array with the non-NULLs in the non-used-array
						m_robots[i]=m_robots[k];
						m_robots[k]=NULL;
					}

    if (m_nRobots < nRobotsOriginally) //output whether player destroyed any robots
        return "Some robots have been destroyed.";
    else
        return "No robots were destroyed.";
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ")" << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRobots)
{
    if (nRobots < 0  ||  nRobots > MAXROBOTS)
    {
        cout << "***** Game created with invalid number of robots:  "
             << nRobots << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nRobots - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nRobots << " robots!" << endl;
        exit(1);
    }

        // Create arena
    m_arena = new Arena(rows, cols);

        // Add some walls in WALL_DENSITY of the empty spots
    assert(WALL_DENSITY >= 0  &&  WALL_DENSITY <= 1);
    int nWalls = static_cast<int>(WALL_DENSITY * nEmpty);
    while (nWalls > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->getCellStatus(r, c) == WALL)
            continue;
        m_arena->setCellStatus(r, c, WALL);
        nWalls--;
    }

        // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with robots
    while (nRobots > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->getCellStatus(r,c) != EMPTY  ||  (r == rPlayer && c == cPlayer))
            continue;
        m_arena->addRobot(r, c, randInt(1, MAXCHANNELS));
        nRobots--;
    }
}

Game::~Game()
{
    delete m_arena; //return memory to computer
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
                return player->stand();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->stand();
            else if (charToDir(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

string Game::takeRobotsTurn()
{
    for (;;)
    {
        cout << "Broadcast (e.g., 2n): ";
        string broadcast;
        getline(cin, broadcast);
        if (broadcast.size() != 2)
            cout << "Broadcast must be channel followed by direction." << endl;
        else if (broadcast[0] < '1'  ||  broadcast[0] > '0'+MAXCHANNELS)
            cout << "Channel must be a valid digit." << endl;
        else
        {
            int dir;
            if (charToDir(broadcast[1], dir))
                return m_arena->moveRobots(broadcast[0]-'0', dir);
            else
                cout << "Direction must be n, e, s, or w." << endl;
        }
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->robotCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);	
        Player* player = m_arena->player();
        if (player->isDead())
            break;
        msg = takeRobotsTurn();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
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
    return lowest + (rand() % (highest - lowest + 1));
}

bool charToDir(char ch, int& dir)
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
	int tempR = r;
	int tempC = c;

	switch(dir) //set the to-be-position (row/col) of the object in the target direction
	{
		case 0: //north, so move up one row
			tempR--;
			break;
		case 1: //east, so move right one col
			tempC++;
			break;
		case 2: //south, so move down one row
			tempR++;
			break;
		case 3: //west, so move left one col
			tempC--;
			break;
		default: //none, so do nothing
			break;
	}

	/*
    bool outOfGrid = (tempR<1 || tempR>a.rows() || tempC<1 || tempC>a.cols());
	
	if(outOfGrid) //check if target location is out of grid
		return false; //if so, we cannot move there

	bool inWall = (a.getCellStatus(tempR, tempC)==WALL); //checkPos should not give error; at this point, target location should be in grid

	if (inWall) //check if target location has a wall
		return false; //if so, we cannot move there
	*/

	
	if(outOfGrid(a, tempR, tempC)) //check if target location is out of grid
		return false; //if so, we cannot move there

	//checkPos in inWall() method should not give error; at this point, target location should be guaranteed to be in grid

	if(inWall(a, tempR, tempC)) //check if target location has a wall
		return false; //if so, we cannot move there


	//otherwise, if the move is possible, we update the row and column, then return true
	r = tempR;
	c = tempC;

	return true;
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should stand; otherwise, bestDir is
  // set to the recommended direction to move.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	int robotsInDir[5]; //find number of robots around each position in all directions

	//North:
	if(!outOfGrid(a, r-1, c) && !inWall(a, r-1, c) && a.numberOfRobotsAt(r-1, c)<1) //cannot be occupied, out of grid, or have a wall
		robotsInDir[0]=numRobotsAround(a, r-1, c); //set corresponding element in array to number of robots around it
	else
		robotsInDir[0]=99; //arbitrary number showing that that position is out of grid/in wall

	//East:
	if(!outOfGrid(a, r, c+1) && !inWall(a, r, c+1) && a.numberOfRobotsAt(r, c+1)<1) //cannot be occupied, out of grid, or have a wall
		robotsInDir[1]=numRobotsAround(a, r, c+1); //set corresponding element in array to number of robots around it
	else
		robotsInDir[1]=99; //arbitrary number showing that that position is out of grid/in wall

	//South:
	if(!outOfGrid(a, r+1, c) && !inWall(a, r+1, c) && a.numberOfRobotsAt(r+1, c)<1) //cannot be occupied, out of grid, or have a wall
		robotsInDir[2]=numRobotsAround(a, r+1, c); //set corresponding element in array to number of robots around it
	else
		robotsInDir[2]=99; //arbitrary number showing that that position is out of grid/in wall

	//West:
	if(!outOfGrid(a, r, c-1) && !inWall(a, r, c-1) && a.numberOfRobotsAt(r, c-1)<1) //cannot be occupied, out of grid, or have a wall
		robotsInDir[3]=numRobotsAround(a, r, c-1); //set corresponding element in array to number of robots around it
	else
		robotsInDir[3]=99; //arbitrary number showing that that position is out of grid/in wall

	robotsInDir[4]=numRobotsAround(a, r, c); //set element in array to number of robots around current position

	int minRobots=robotsInDir[0]; //set counter for minimum number of robots around a certain position
	int index=0; //set an index to go through all position positions
	int minIndex=0; //set an index that corresponds to the direction with least robots around it

	while(index<NUMDIRS+1)
	{
		if(robotsInDir[index]<=minRobots)
		{
			minRobots=robotsInDir[index]; //determine direction surrounded by fewest robots
			minIndex=index;
		}
		index++;
	}

	if(minIndex==4) //standing would be the best option, as it has the least numRobotsAround
		return false; //return false so that player does not move
	else
	{
		bestDir = minIndex; //set best direction to the direction which has the last numRobotsAround
		return true; //return true, since player will move
	}
}

int numRobotsAround(const Arena& a, int r, int c)
{
	int count=0; //initiate counter for the amount of surrounding robots

	//check all four positions and add up all robots (in all four directions) surrounding each position
	if(!outOfGrid(a, r-1, c))
		count+=a.numberOfRobotsAt(r-1, c);

	if(!outOfGrid(a, r+1, c))
		count+=a.numberOfRobotsAt(r+1, c);

	if(!outOfGrid(a, r, c-1))
		count+=a.numberOfRobotsAt(r, c-1);

	if(!outOfGrid(a, r, c+1))
		count+=a.numberOfRobotsAt(r, c+1);

	return count; //return the total number of robots surrounding the position
}

bool outOfGrid(const Arena& a, int r, int c)
{
	return (r<1 || r>a.rows() ||c<1 || c>a.cols()); //return whether the position is out of grid
}

bool inWall(const Arena& a, int r, int c)
{
	return (a.getCellStatus(r, c)==WALL); //return whether the position contains a wall
}

#ifdef _MSC_VER  //  Microsoft Visual C++

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

#include <cstring>

void clearScreen()
{
    static const char* term = getenv("TERM");
    static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
    if (term == NULL  ||  strcmp(term, "dumb") == 0)
        cout << endl;
     else
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
}

#endif

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
      // Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));

      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 50);

      // Play the game
    g.play();
}
