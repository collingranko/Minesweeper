#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"
#include "TextureManager.h"
#include "Random.h"
using std::cout;
using std::endl;

/* ===== Declared Functions ===== */
void drawBoard(sf::RenderWindow& window, Board& board);

int main()
{
    string testFilePath;
    Board theBoard;
    /* ===== Minesweeper ===== */
    configcfg(theBoard);
    mapTiles(theBoard);
    linkTiles(theBoard);
    sf::RenderWindow mSweeper(sf::VideoMode(theBoard.width, theBoard.height), "Minesweeper");
    // White background to make button drawing more clean
    sf::RectangleShape background(sf::Vector2f(theBoard.width, theBoard.height));
    background.setFillColor(sf::Color::White);

    while (mSweeper.isOpen())
    {
        sf::Event event;
        while (mSweeper.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mSweeper.close();
            }    
            /* ===== Right Click Event ===== */
            // Toggle flags
            else if (event.mouseButton.button == sf::Mouse::Right)
            {
                if (theBoard.gameWon || theBoard.gameLoss)
                {
                    // Functionality lock (game is over)
                }
                else
                {
                    // Grabs mouse position
                    // Toggles flag for tile containing the mouse right click press
                    sf::Vector2i position = sf::Mouse::getPosition(mSweeper);
                    for (unsigned int i = 0; i < theBoard.tileCount; i++)
                    {
                        if (theBoard.myTiles[i].tilePNG.getGlobalBounds().contains(sf::Vector2f(position.x, position.y)))
                        {
                            // Ensures toggle occurs only on a press and not on a release
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                            {
                                toggleFlag(theBoard, theBoard.myTiles[i]);
                            }
                        }
                    }
                }
            }
            /* ===== Left Click Event ===== */
            // Flip a tile
            else if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i position = sf::Mouse::getPosition(mSweeper);
                if (theBoard.gameWon || theBoard.gameLoss)
                {
                    // Functionality lock (game is over)
                }
                else 
                {
                    // Grabs mouse position
                    // Toggles debug mode if the button contains the mouse left click press
                    if (theBoard.debugBtn.getGlobalBounds().contains(sf::Vector2f(position.x, position.y)))
                    {
                        toggleDebug(theBoard);
                    }
                    else 
                    {
                        // Grabs mouse position
                        // Attempts to flip tile containing the mouse left click press
                        // Checks for end game conditions
                        for (unsigned int i = 0; i < theBoard.tileCount; i++)
                        {
                            if (theBoard.myTiles[i].tilePNG.getGlobalBounds().contains(sf::Vector2f(position.x, position.y)))
                            {
                                flipATile(theBoard, theBoard.myTiles[i]);
                            }
                        }
                    }
                }
                // Buttons can be triggered regardless of game being over or not
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (theBoard.test1Btn.getGlobalBounds().contains(sf::Vector2f(position.x, position.y)))
                    {
                        // Load test 1
                        testFilePath = "boards/testboard1.brd";
                        getTestBoard(theBoard, testFilePath);
                        mapTiles(theBoard);
                        linkTiles(theBoard);
                    }
                    else if (theBoard.test2Btn.getGlobalBounds().contains(sf::Vector2f(position.x, position.y)))
                    {
                        // Load test 2
                        testFilePath = "boards/testboard2.brd";
                        getTestBoard(theBoard, testFilePath);
                        mapTiles(theBoard);
                        linkTiles(theBoard);
                    }
                    else if (theBoard.test3Btn.getGlobalBounds().contains(sf::Vector2f(position.x, position.y)))
                    {
                        // Load test 3
                        testFilePath = "boards/testboard3.brd";
                        getTestBoard(theBoard, testFilePath);
                        mapTiles(theBoard);
                        linkTiles(theBoard);
                    }
                    else if (theBoard.resetBtn.getGlobalBounds().contains(sf::Vector2f(position.x, position.y)))
                    {
                        toggleReset(theBoard);
                        mapTiles(theBoard);
                        linkTiles(theBoard);
                    }
                }
            }
        }
        mSweeper.clear();
        mSweeper.draw(background);
        drawBoard(mSweeper, theBoard);
        mSweeper.display();
    }
    TextureManager::Clear(); // Clean up textures
    return 0;
}

/* ===== Main Definitions ===== */
void drawBoard(sf::RenderWindow& window, Board& board)
{
    float xVal = 0;
    float yVal = 0;
    int trackTile = 0;
    for (unsigned int i = 0; i < board.rows; i++)
    {
        for (unsigned int j = 0; j < board.columns; j++)
        {
            // Draw Tiles
            // Here tiles are drawn like a book (left to right, top to bottom)
            // if hidden check for flag
            // if revealed check for adjacent mines
            // if game is over overrite and reveal all mines
            if (board.myTiles[trackTile].tileHidden)
            {
                board.myTiles[trackTile].tilePNG.setPosition(xVal, yVal);
                window.draw(board.myTiles[trackTile].tilePNG);
                if (board.myTiles[trackTile].tileFlag)
                {
                    board.myTiles[trackTile].tilePNGFlag.setPosition(xVal, yVal);
                    window.draw(board.myTiles[trackTile].tilePNGFlag);
                }
            }
            if (!board.myTiles[trackTile].tileHidden)
            {
                board.myTiles[trackTile].tilePNG.setPosition(xVal, yVal);
                window.draw(board.myTiles[trackTile].tilePNG);
                if (board.myTiles[trackTile].tileMine)
                {
                    board.myTiles[trackTile].tilePNGMine.setPosition(xVal, yVal);
                    window.draw(board.myTiles[trackTile].tilePNGMine);
                }
                else if (board.myTiles[trackTile].adjacentMines != 0)
                {
                    tileNumber(board.myTiles[trackTile]);
                    board.myTiles[trackTile].tilePNGNumber.setPosition(xVal, yVal);
                    window.draw(board.myTiles[trackTile].tilePNGNumber);
                }
            }
            // Debug mode - write mines over tiles
            if (board.debugMode)
            {
                if (board.myTiles[trackTile].tileMine)
                {
                    board.myTiles[trackTile].tilePNGMine.setTexture(TextureManager::GetTexture("mine"));
                    board.myTiles[trackTile].tilePNGMine.setPosition(xVal, yVal);
                    window.draw(board.myTiles[trackTile].tilePNGMine);
                }
            }
            // Game is lost
            // All hidden mines to  be flipped and written
            if (board.gameLoss)
            {
                if (board.myTiles[trackTile].tileMine)
                {
                    board.myTiles[trackTile].tileHidden = false;
                    board.myTiles[trackTile].tilePNG.setTexture(TextureManager::GetTexture("tile_revealed"));
                    board.myTiles[trackTile].tilePNG.setPosition(xVal, yVal);
                    window.draw(board.myTiles[trackTile].tilePNG);
                    if (board.myTiles[trackTile].tileFlag)
                    {
                        board.myTiles[trackTile].tilePNGFlag.setPosition(xVal, yVal);
                        window.draw(board.myTiles[trackTile].tilePNGFlag);
                    }
                    board.myTiles[trackTile].tilePNGMine.setTexture(TextureManager::GetTexture("mine"));
                    board.myTiles[trackTile].tilePNGMine.setPosition(xVal, yVal);
                    window.draw(board.myTiles[trackTile].tilePNGMine);
                }
            }
            xVal += 32;
            trackTile++;
        }
        xVal = 0;
        yVal += 32;
    }
    // Draw Buttons
    board.resetBtn.setPosition((board.width / 2 - 32), yVal);
    window.draw(board.resetBtn);

    board.test1Btn.setPosition((board.width - 192), yVal);
    window.draw(board.test1Btn);

    board.test2Btn.setPosition((board.width - 128), yVal);
    window.draw(board.test2Btn);

    board.test3Btn.setPosition((board.width - 64), yVal);
    window.draw(board.test3Btn);

    board.debugBtn.setPosition((board.width - 256), yVal);
    window.draw(board.debugBtn);

    board.counterBtn3.setPosition(21, yVal);
    board.counterBtn2.setPosition(42, yVal);
    board.counterBtn1.setPosition(63, yVal);
    board.counterBtnN.setPosition(0, yVal);

    minesRemaining(board);
    counter(board, board.counterBtn1, 0);          
    counter(board, board.counterBtn2, 1);
    counter(board, board.counterBtn3, 2);
    counter(board, board.counterBtnN, 2);
    board.counterBtnN.setTextureRect(sf::IntRect(210, 0, 21, 32));

    window.draw(board.counterBtn1);
    window.draw(board.counterBtn2);
    window.draw(board.counterBtn3);
    if (board.flagMineCount > board.mineCount)
    {
        window.draw(board.counterBtnN);
    }
}

// Rubric Tests
// 1 - done
// 2 - done
// 3 - done
// 4 - done
// 5 - done
// 6 - done
// 7 - done
// 8 - done
// 9 - done
// 10 - done