#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tile.h"
#include "Random.h"
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::getline;
using std::string;
using std::stoi;

/* ===== Board Class ===== */
class Board
{
public:
	/* ===== Board Variables ===== */
	Board();					// Default constructor
	unsigned int rows;			// Number of rows of tiles
	unsigned int columns;		// Number of columns of tiles
	unsigned int width;			// Width in pixels: columns * 32
	unsigned int height;		// Height in pixels: (rows * 32) + 88
	unsigned int mineCount;		// Mines to be put on the board
	unsigned int tileCount;		// Total tiles: columns * rows
	int flagMineCount;			// Tracks number of flags to be compared to number of mines
	vector<Tile> myTiles;		// Vector of all tiles (but not buttons)
	bool gameLoss;				// Alters board and tile conditions incase of a loss
	bool gameWon;				// Alters board and tile conditions incase of a win
	bool debugMode;				// Toggle showing of bombs

	// Balnk tile to hold pointers
	// helps when assigning neighboring tiles
	Tile gMas;
	
	// Board Button Sprites
	sf::Sprite resetBtn;
	sf::Sprite test1Btn;
	sf::Sprite test2Btn;
	sf::Sprite test3Btn;
	sf::Sprite debugBtn;
	sf::Sprite counterBtn1;
	sf::Sprite counterBtn2;
	sf::Sprite counterBtn3;
	sf::Sprite counterBtnN;
};

/* ===== Board Creation Declarations ===== */
void configcfg(Board& config);									// Load config.cfg board function
void getTestBoard(Board& testBoard, string filePath);			// Load Test Board Functions

/* ===== Button Functionality Declarations ===== */
void toggleDebug(Board& board);									// Handles debug button
void showAllMines(Board& board);								// Reveal all bombs
void hideAllMines(Board& board);								// Hide all bombs
void toggleReset(Board& board);									// Handles reset button
void minesRemaining(Board& board);								// Creates and maintains mine counter
void counter(Board& board, sf::Sprite& btn, int index);

/* ===== End Conditions Declarations ===== */
void checkEndConditions(Board& board, Tile& tile);				// Test end of game condition
bool isGameWon(Board& board);									// Testing if the game is won

/* ===== Tile Manipulation Declarations ===== */
void toggleFlag(Board& board, Tile& tile);						// Toggle flag on hidden tiles
void flipATile(Board& board, Tile& tile);						// Flips hidden tiles
void mapTiles(Board& board);									// Maps tiles for linking
void linkTiles(Board& board);									// Links neighboring tiles if any
Tile& findTile(Board& board, unsigned int x, unsigned int y);	// Returns the at the given coordinates

/* ===== Board and Board Related Definitions ===== */
Board::Board() 
{
	// constructor
	gMas = Tile();
}
void configcfg(Board& config)
{
	// 1. Open cfg file
	// 2. Read columns, rows, and mines
	// 3. Assign to the board object
	// 4. Create buttons, tiles, and mines
	// Board config;
	config.myTiles.clear();							// Clears the vector when new board is loaded
	ifstream configBoard("boards/config.cfg");
	if (configBoard.is_open())
	{
		// Read file
		string columnNum;
		string rowNum;
		string mines;
		getline(configBoard, columnNum);
		getline(configBoard, rowNum);
		getline(configBoard, mines);

		// Store board info
		config.width = stoi(columnNum) * 32;
		config.height = (stoi(rowNum) * 32) + 88;
		config.mineCount = stoi(mines);
		config.tileCount = stoi(columnNum) * stoi(rowNum);
		config.rows = stoi(rowNum);
		config.columns = stoi(columnNum);
		config.flagMineCount = 0;
		config.gameLoss = false;
		config.gameWon = false;
		config.debugMode = config.debugMode;

		// Assign intial button sprites
		config.test1Btn.setTexture(TextureManager::GetTexture("test_1"));
		config.test2Btn.setTexture(TextureManager::GetTexture("test_2"));
		config.test3Btn.setTexture(TextureManager::GetTexture("test_3"));
		config.resetBtn.setTexture(TextureManager::GetTexture("face_happy"));
		config.debugBtn.setTexture(TextureManager::GetTexture("debug"));
		config.counterBtn1.setTexture(TextureManager::GetTexture("digits"));
		config.counterBtn2.setTexture(TextureManager::GetTexture("digits"));
		config.counterBtn3.setTexture(TextureManager::GetTexture("digits"));
		config.counterBtnN.setTexture(TextureManager::GetTexture("digits"));
		
		// Create tiles with default constructor
		for (unsigned int i = 0; i < config.tileCount; i++)
		{
			config.myTiles.push_back(Tile());
			for (unsigned int j = 0; j < 8; j++)
			{
				config.myTiles[i].neighbors[j] = &config.gMas;
			}
		}
		
		// Assign mines using random number generator
		for (unsigned int i = 0; i < config.mineCount; i++)
		{
			int mineHere = Random::Int(0, config.myTiles.size() - 1);
			if (config.myTiles[mineHere].tileMine == true)
			{
				i--; // if there is a mine already here lets counter the increment
			}
			else
			{
				config.myTiles[mineHere].tileMine = true;
			}
		}
		configBoard.close();
	}
}
void getTestBoard(Board& testBoard, string filePath)
{
	// 1. Open testboard file
	// 2. Copy cfg variables
	// 3. Assign to the board object
	// 4. Read for mines
	testBoard.myTiles.clear();							// Clears the vector when new board is loaded
	ifstream loadTestBoard(filePath);
	if (loadTestBoard.is_open())
	{
		testBoard.mineCount = 0;
		testBoard.flagMineCount = 0;
		testBoard.gameLoss = false;
		testBoard.gameWon = false;
		testBoard.debugMode = testBoard.debugMode;

		// Create tiles with default constructor
		for (unsigned int i = 0; i < testBoard.tileCount; i++)
		{
			testBoard.myTiles.push_back(Tile());
			for (unsigned int j = 0; j < 8; j++)
			{
				testBoard.myTiles[i].neighbors[j] = &testBoard.gMas;
			}
		}

		// Read file for mines
		char value;
		int b = 0;
		while (loadTestBoard >> value)
		{
			if (value == '1')
			{
				testBoard.myTiles[b].tileMine = true;
				testBoard.mineCount++;
			}
			b++;
		}
		testBoard.resetBtn.setTexture(TextureManager::GetTexture("face_happy"));
		// Close file
		loadTestBoard.close();
	}
}
void toggleDebug(Board& board)
{
	// Ensures toggle occurs only on a press and not on a release
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!board.debugMode)
		{
			board.debugMode = true;
		}
		else
		{
			board.debugMode = false;
		}
	}
}
void showAllMines(Board& board)
{
	for (unsigned int i = 0; i < board.myTiles.size(); i++)
	{
		if (board.myTiles[i].tileMine)
		{
			board.myTiles[i].tilePNGMine.setTexture(TextureManager::GetTexture("mine"));
		}
	}
}
void hideAllMines(Board& board)
{
	for (unsigned int i = 0; i < board.myTiles.size(); i++)
	{
		if (board.myTiles[i].tileMine)
		{
			board.myTiles[i].tileHidden = true;
			board.myTiles[i].tilePNG.setTexture(TextureManager::GetTexture("tile_hidden"));
		}
	}
}
void toggleReset(Board& board)
{
	// Reset the board
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		configcfg(board);
	}
}
void checkEndConditions(Board& board, Tile& tile)
{
	// Lose condition
	if (tile.tileMine)
	{
		board.gameLoss = true;
		showAllMines(board);
		board.resetBtn.setTexture(TextureManager::GetTexture("face_lose"));
	}
	// Win condition 
	else if (isGameWon(board))
	{
		board.gameWon = true;
		board.resetBtn.setTexture(TextureManager::GetTexture("face_win"));
	}
}
bool isGameWon(Board& board)
{
	// If a hidden non-mine tile exists then game is not won
	for (unsigned int i = 0; i < board.myTiles.size(); i++)
	{
		if (board.myTiles[i].tileHidden == true && board.myTiles[i].tileMine == false)
		{
			return false;
		}
	}
	for (unsigned int i = 0; i < board.myTiles.size(); i++)
	{
		if (board.myTiles[i].tileMine)
		{
			if (!board.myTiles[i].tileFlag)
			{
				board.myTiles[i].tilePNGFlag.setTexture(TextureManager::GetTexture("flag"));
				board.myTiles[i].tileFlag = true;
			}
		}
	}
	return true;
}
void toggleFlag(Board& board, Tile& tile)
{
	if (tile.tileHidden)
	{
		if (!tile.tileFlag)
		{
			tile.tilePNGFlag.setTexture(TextureManager::GetTexture("flag"));
			tile.tileFlag = true;
		}
		else if (tile.tileFlag)
		{
			tile.tileFlag = false;
		}
	}
}
void flipATile(Board& board, Tile& tile)
{
	if (!tile.tileFlag)
	{
		if (tile.tileHidden)
		{
			tile.tileHidden = false;
			tile.tilePNG.setTexture(TextureManager::GetTexture("tile_revealed"));

			// Check both lose and win conditions
			checkEndConditions(board, tile);

			if (!tile.tileMine)
			{
				countAdjacentMines(tile);
				if (tile.adjacentMines == 0)
				{
					for (unsigned int i = 0; i < 8; i++)
					{
						if (tile.neighbors[i] != nullptr)
						{
							flipATile(board, *tile.neighbors[i]);
						}
					}
				}
			}
		}
	}
}
void linkTiles(Board& board)
{
	// 0 - 8 ordered left to right, top to bottom 
	// Nullptr if tile is not there
	for (unsigned int i = 0; i < board.myTiles.size(); i++)
	{
		// Border tiles
		// For top row tiles - 0, 1, 2
		if (board.myTiles[i].y == 0)
		{
			board.myTiles[i].neighbors[0] = nullptr;
			board.myTiles[i].neighbors[1] = nullptr;
			board.myTiles[i].neighbors[2] = nullptr;
		}
		// For bottom row tiles - 5, 6, 7
		if (board.myTiles[i].y == board.rows - 1)
		{
			board.myTiles[i].neighbors[5] = nullptr;
			board.myTiles[i].neighbors[6] = nullptr;
			board.myTiles[i].neighbors[7] = nullptr;
		}
		// For left border tiles - 0, 3, 5
		if (board.myTiles[i].x == 0)
		{
			board.myTiles[i].neighbors[0] = nullptr;
			board.myTiles[i].neighbors[3] = nullptr;
			board.myTiles[i].neighbors[5] = nullptr;
		}
		// For right border tiles - 2, 4, 7
		if (board.myTiles[i].x == board.columns - 1)
		{
			board.myTiles[i].neighbors[2] = nullptr;
			board.myTiles[i].neighbors[4] = nullptr;
			board.myTiles[i].neighbors[7] = nullptr;
		}
		// Regular tiles
		unsigned int xPos = 0;
		unsigned int yPos = 0;
		if (board.myTiles[i].neighbors[0] != nullptr)
		{
			xPos = board.myTiles[i].x - 1;
			yPos = board.myTiles[i].y - 1;
			board.myTiles[i].neighbors[0] = &findTile(board, xPos, yPos);
		}
		if (board.myTiles[i].neighbors[1] != nullptr)
		{
			xPos = board.myTiles[i].x;
			yPos = board.myTiles[i].y - 1;
			board.myTiles[i].neighbors[1] = &findTile(board, xPos, yPos);
		}
		if (board.myTiles[i].neighbors[2] != nullptr)
		{
			xPos = board.myTiles[i].x + 1;
			yPos = board.myTiles[i].y - 1;
			board.myTiles[i].neighbors[2] = &findTile(board, xPos, yPos);
		}
		if (board.myTiles[i].neighbors[3] != nullptr)
		{
			xPos = board.myTiles[i].x - 1;
			yPos = board.myTiles[i].y;
			board.myTiles[i].neighbors[3] = &findTile(board, xPos, yPos);
		}
		if (board.myTiles[i].neighbors[4] != nullptr)
		{
			xPos = board.myTiles[i].x + 1;
			yPos = board.myTiles[i].y;
			board.myTiles[i].neighbors[4] = &findTile(board, xPos, yPos);
		}
		if (board.myTiles[i].neighbors[5] != nullptr)
		{
			xPos = board.myTiles[i].x - 1;
			yPos = board.myTiles[i].y + 1;
			board.myTiles[i].neighbors[5] = &findTile(board, xPos, yPos);
		}
		if (board.myTiles[i].neighbors[6] != nullptr)
		{
			xPos = board.myTiles[i].x;
			yPos = board.myTiles[i].y + 1;
			board.myTiles[i].neighbors[6] = &findTile(board, xPos, yPos);
		}
		if (board.myTiles[i].neighbors[7] != nullptr)
		{
			xPos = board.myTiles[i].x + 1;
			yPos = board.myTiles[i].y + 1;
			board.myTiles[i].neighbors[7] = &findTile(board, xPos, yPos);
		}
	}
}
Tile& findTile(Board& board, unsigned int x, unsigned int y)
{
	for (unsigned int t = 0; t < board.myTiles.size(); t++)
	{
		if (board.myTiles[t].x == x && board.myTiles[t].y == y)
		{
			return board.myTiles[t];
		}
	}
}
void mapTiles(Board& board)
{
	int increase = 0;
	int tileX = 0;
	int tileY = 0;
	for (unsigned int p = 0; p < board.rows; p++)
	{
		increase = p * board.columns;
		tileX = 0;
		for (unsigned int o = 0; o < board.columns; o++)
		{
			board.myTiles[o + increase].x = tileX;
			board.myTiles[o + increase].y = tileY;
			tileX++;
		}
		tileY++;
	}
}
void minesRemaining(Board& board)
{
	board.flagMineCount = 0;
	for (unsigned int i = 0; i < board.myTiles.size(); i++)
	{
		if (board.myTiles[i].tileFlag)
		{
			board.flagMineCount++;
		}
	}
}
void counter(Board& board, sf::Sprite& btn, int index)
{
	// Mines on board - flags on board
	int num = board.mineCount - board.flagMineCount;
	num = abs(num);
	string digits = to_string(num);
	if (digits.size() - 1 >= index)
	{
		index = (digits.size() - 1) - index;
		if (digits.at(index) == '0')
		{
			btn.setTextureRect(sf::IntRect(0, 0, 21, 32));
		}
		else if (digits.at(index) == '1')
		{
			btn.setTextureRect(sf::IntRect(21, 0, 21, 32));
		}
		else if (digits.at(index) == '2')
		{
			btn.setTextureRect(sf::IntRect(42, 0, 21, 32));
		}
		else if (digits.at(index) == '3')
		{
			btn.setTextureRect(sf::IntRect(63, 0, 21, 32));
		}
		else if (digits.at(index) == '4')
		{
			btn.setTextureRect(sf::IntRect(84, 0, 21, 32));
		}
		else if (digits.at(index) == '5')
		{
			btn.setTextureRect(sf::IntRect(105, 0, 21, 32));
		}
		else if (digits.at(index) == '6')
		{
			btn.setTextureRect(sf::IntRect(126, 0, 21, 32));
		}
		else if (digits.at(index) == '7')
		{
			btn.setTextureRect(sf::IntRect(147, 0, 21, 32));
		}
		else if (digits.at(index) == '8')
		{
			btn.setTextureRect(sf::IntRect(168, 0, 21, 32));
		}
		else if (digits.at(index) == '9')
		{
			btn.setTextureRect(sf::IntRect(189, 0, 21, 32));
		}
	}
	else
	{
		// If the array size is not big enough, 0 is assigned to the place
		btn.setTextureRect(sf::IntRect(0, 0, 21, 32));
	}
	// if counter ever reaches above/below 999
	if (digits.size() >= 4)
	{
		btn.setTextureRect(sf::IntRect(189, 0, 21, 32));
	}
}