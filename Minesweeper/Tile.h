#pragma once
#include <vector>
#include "TextureManager.h"
#include "Board.h"
using std::vector;
using std::to_string;

/* ===== Tile Class ===== */
class Tile
{
public:
	/* ===== Tile Variables && Functions ===== */
	Tile();									// Default constructor the game tiles

	// Vector of pointers to neighboring tiles
	Tile* neighbors[8];						// Array of pointers to neighboring tiles
	int adjacentMines;						// How many of those neighbors have mines

	// Booleans, Mines, and Flags
	bool tileHidden;						// Is tile hidden
	bool tileMine;							// Does it have  a mine
	bool tileFlag;							// Is it flagged

	// Location for keeping track on the board
	unsigned int x;							// X coordinate (see map)
	unsigned int y;							// Y coordinate (see map)

	// Sprite
	sf::Sprite tilePNG;
	sf::Sprite tilePNGFlag;
	sf::Sprite tilePNGMine;
	sf::Sprite tilePNGNumber;
};

/* ===== Tile Manipulation Declarations ===== */
void countAdjacentMines(Tile& tile);
void tileNumber(Tile& tile);

/* ===== Tile Manipulation Definitions ===== */
void countAdjacentMines(Tile& tile)
{
	tile.adjacentMines = 0;
	for (unsigned int i = 0; i < 8; i++)
	{
		if (tile.neighbors[i] != nullptr)
		{
			if (tile.neighbors[i]->tileMine)
			{
				tile.adjacentMines++;
			}
		}
	}
}
void tileNumber(Tile& tile)
{
	string numberPath = "number_";
	numberPath.append(to_string(tile.adjacentMines));
	tile.tilePNGNumber.setTexture(TextureManager::GetTexture(numberPath));
}

/* ===== Tile Definitions ===== */
Tile::Tile()
{
	// Creates a hidden tile without a flag or mine
	tileHidden = true;
	tileMine = false;
	tileFlag = false;
	tilePNG.setTexture(TextureManager::GetTexture("tile_hidden"));
	adjacentMines = 0;
	x = 0;
	y = 0;
	for (unsigned int i = 0; i < 8; i++)
	{
		neighbors[i] = nullptr;
	}
}