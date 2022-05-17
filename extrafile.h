#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
struct Tile
{
    bool blank = true;
    bool mine = false;
    bool flag = false;
    bool revealed = false;
    int numAdjacent = 0;
    Tile* adjacentTiles[8];
};
class Board
{
    int mines;
    int numTiles;
    int columns;
    int rows;
    int width;
    int height;
    bool gameOver = false;
    bool smiley = false;
    bool debug = false;
    bool test1 = false;
    bool test2 = false;
    bool test3 = false;
    bool win = false;
public:
    Tile** tiles;
    
    Board(int columns, int rows, int mines, int numTiles, int width, int height);
    Board(const Board& original);
    Board& operator=(const Board& original);
    ~Board();
    void SetupTiles();
    void DrawBoard(sf::RenderWindow& window);
    void DrawSprite(std::string png, float x, float y, sf::RenderWindow& window);
    void PlaceMines();
    void PlaceAdjacent();
    void BoardReveal(const int &x, const int &y);
    void ButtonPress(const int& x, const int& y);
    void PlaceFlag(const int& x, const int& y);
    void BlankReveal(const Tile& tile);
    void UpdateCounter(std::vector<sf::Texture>& digits, sf::RenderWindow& window);
    int GetHeight();
    bool GetGameOver();
    bool GetSmiley();
    bool GetTest1();
    bool GetTest2();
    bool GetTest3();
    void CheckWin();
    void LoadMines(std::ifstream& file);
};
void LoadDigits(std::vector<sf::Texture>& digits);
Board LoadBoard(std::ifstream& file);