#include "extrafile.h"
#include <string>
using namespace std;
Board::Board(int columns, int rows, int mines, int numTiles, int width, int height)
{
	this->mines = mines;
	this->numTiles = numTiles;
	this->columns = columns;
	this->rows = rows;
	this->width = width;
	this->height = height;
	this->SetupTiles();
}
Board::Board(const Board& original)
{
	mines = original.mines;
	numTiles = original.numTiles;
	columns = original.columns;
	rows = original.rows;
	width = original.width;
	height = original.height;
	tiles = new Tile* [columns];
	gameOver = original.gameOver;
	smiley = original.smiley;
	debug = original.debug;
	test1 = original.test1;
	test2 = original.test2;
	test3 = original.test3;
	win = original.win;
	this->SetupTiles();
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			tiles[i][j] = original.tiles[i][j];
		}
	}
}
Board& Board::operator=(const Board& original)
{
	if (this != &original) 
	{
		this->~Board();
		mines = original.mines;
		numTiles = original.numTiles;
		columns = original.columns;
		rows = original.rows;
		width = original.width;
		height = original.height;
		tiles = new Tile* [columns];
		gameOver = original.gameOver;
		smiley = original.smiley;
		debug = original.debug;
		test1 = original.test1;
		test2 = original.test2;
		test3 = original.test3;
		win = original.win;
		this->SetupTiles();
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				tiles[i][j] = original.tiles[i][j];
			}
		}
	}
	return *this;
}
Board::~Board() 
{
	for (int i = 0; i < columns; i++)
	{
		delete[] tiles[i];
	}
	delete[] tiles;
}
void Board::SetupTiles()
{
	tiles = new Tile * [columns];
	for (int i = 0; i < columns; i++)
	{
		tiles[i] = new Tile[rows];
	}
}

void Board::DrawBoard(sf::RenderWindow& window)
{
	window.clear(sf::Color(255, 255, 255, 255));
	sf::Texture hiddenTileTexture;
	hiddenTileTexture.loadFromFile("images/tile_hidden.png");
	sf::Sprite hiddenTile(hiddenTileTexture);
	for (int i = 0; i < columns; ++i)
	{
		for (int j = 0; j < rows; j++) 
		{
			if (!tiles[i][j].revealed)
			{
				DrawSprite("images/tile_hidden.png", (float)i * 32, (float)j * 32, window);
				if (tiles[i][j].flag)
				{
					DrawSprite("images/flag.png", (float)i * 32, (float)j * 32, window);
				}
				if (debug && tiles[i][j].mine)
				{
					DrawSprite("images/mine.png", (float)i * 32, (float)j * 32, window);
				}
			}
			else if (tiles[i][j].mine)
			{
				DrawSprite("images/tile_revealed.png", (float)i * 32, (float)j * 32, window);
				if (tiles[i][j].flag)
				{
					DrawSprite("images/flag.png", (float)i * 32, (float)j * 32, window);
				}
				DrawSprite("images/mine.png", (float)i * 32, (float)j * 32, window);

			}
			else if (tiles[i][j].numAdjacent > 0)
			{
				std::string png = "images/number_" + std::to_string(tiles[i][j].numAdjacent) + ".png";
				DrawSprite("images/tile_revealed.png", (float)i * 32, (float)j * 32, window);
				DrawSprite(png, (float)i * 32, (float)j * 32, window);
			}
			else 
			{
				DrawSprite("images/tile_revealed.png", (float)i * 32, (float)j * 32, window);
			}
		}
	}
	int smileyX = (width / 2) - 32;
	int smileyY = (height - 88);
	if (gameOver && win)
	{
		DrawSprite("images/face_win.png", (float)smileyX, (float)smileyY, window);
	}
	else if (gameOver && !win)
	{
		DrawSprite("images/face_lose.png", (float)smileyX, (float)smileyY, window);
	}
	else
	{
		DrawSprite("images/face_happy.png", (float)smileyX, (float)smileyY, window);
	}
	DrawSprite("images/debug.png", (float)(smileyX + 128), (float)smileyY, window);
	DrawSprite("images/test_1.png", (float)(smileyX + 192), (float)smileyY, window);
	DrawSprite("images/test_2.png", (float)(smileyX + 256), (float)smileyY, window);
	DrawSprite("images/test_3.png", (float)(smileyX + 320), (float)smileyY, window);
}
void Board::DrawSprite(std::string png, float x, float y, sf::RenderWindow& window)
{
	sf::Texture texture;
	texture.loadFromFile(png);
	sf::Sprite sprite(texture);
	sprite.setPosition(x, y);
	window.draw(sprite);
}
void Board::PlaceMines()
{
	for (int i = 0; i < mines; i++)
	{
		int x;
		int y;
		do
		{
			x = rand() % columns;
			y = rand() % rows;
		} 
		while (tiles[x][y].mine);
		tiles[x][y].mine = true;
		tiles[x][y].blank = false;
	}
}
void Board::PlaceAdjacent()
{
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			int count = 0;
			if ((j - 1) < 0)
			{
				tiles[i][j].adjacentTiles[count] = nullptr;
			}
			else
			{
				tiles[i][j].adjacentTiles[count] = &(tiles[i][j - 1]);
				if (tiles[i][j].adjacentTiles[count]->mine)
				{
					tiles[i][j].numAdjacent++;
				}
			}
			count++;
			for (int k = -1; k < 2; k++)
			{
				int y = j + k;
				if (y < 0 || i >= (columns - 1) || y > (rows - 1))
				{
					tiles[i][j].adjacentTiles[count] = nullptr;
				}
				else 
				{
					tiles[i][j].adjacentTiles[count] = &(tiles[i + 1][y]);
					if (tiles[i][j].adjacentTiles[count]->mine)
					{
						tiles[i][j].numAdjacent++;
					}
				}
				count++;
			}
			if ((j + 1) >= (rows))
			{
				tiles[i][j].adjacentTiles[count] = nullptr;
			}
			else
			{
				tiles[i][j].adjacentTiles[count] = &(tiles[i][j + 1]);
				if (tiles[i][j].adjacentTiles[count]->mine)
				{
					tiles[i][j].numAdjacent++;
				}
			}
			count++;
			for (int k = 1; k > -2; k--)
			{
				int y = j + k;
				if (y >= (rows) || i <= 0 || y < 0)
				{
					tiles[i][j].adjacentTiles[count] = nullptr;
				}
				else
				{
					tiles[i][j].adjacentTiles[count] = &(tiles[i - 1][y]);
					if (tiles[i][j].adjacentTiles[count]->mine)
					{
						tiles[i][j].numAdjacent++;
					}
				}
				count++;
			}
			if (tiles[i][j].numAdjacent > 0)
			{
				tiles[i][j].blank = false;
			}
		}
	}
}
void Board::BoardReveal(const int& x, const int& y)
{
	int i = x / 32;
	int j = y / 32;
	if (i < 0 || i > (columns - 1))
	{
		return;
	}
	if (j < 0 || j >(rows - 1))
	{
		return;
	}
	if (tiles[i][j].blank && (!tiles[i][j].revealed) && (!tiles[i][j].flag))
	{
		BlankReveal(tiles[i][j]);
		tiles[i][j].revealed = true;
	}
	if (tiles[i][j].mine && !tiles[i][j].flag)
	{
		gameOver = true;
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
			{
				if (tiles[i][j].mine)
				{
					tiles[i][j].revealed = true;
				}
			}
		}
	}
	if (tiles[i][j].numAdjacent > 0)
	{
		tiles[i][j].revealed = true;
	}
}
void Board::ButtonPress(const int& x, const int& y)
{
	int smileyX = (width / 2) - 32;
	int smileyY = (height - 88);
	
	if (x >= smileyX && x <= (smileyX + 64) && gameOver)
	{
		smiley = true;
	}
	if (x >= smileyX + 128 && x < (smileyX + 192) && !gameOver) 
	{
		if (debug)
		{
			debug = false;
		}
		else
		{
			debug = true;
		}
	}
	if (x >= smileyX + 192 && x < (smileyX + 256))
	{
		test1 = true;
	}
	if (x >= smileyX + 256 && x < (smileyX + 320))
	{
		test2 = true;
	}
	if (x >= smileyX + 320 && x < (smileyX + 384))
	{
		test3 = true;
	}
}
void Board::PlaceFlag(const int& x, const int& y)
{
	int i = x / 32;
	int j = y / 32;
	if (!(tiles[i][j].revealed) && tiles[i][j].flag)
	{
		tiles[i][j].flag = false;
		mines++;
	}
	else if (!(tiles[i][j].revealed) && !(tiles[i][j].flag))
	{
		tiles[i][j].flag = true;
		mines--;
	}
}
void Board::BlankReveal(const Tile& tile)
{
	for (int i = 0; i < 8; i++)
	{
		if (tile.adjacentTiles[i] != nullptr)
		{
			if (tile.adjacentTiles[i]->blank && !tile.adjacentTiles[i]->flag)
			{
				if (!tile.adjacentTiles[i]->revealed)
				{
					tile.adjacentTiles[i]->revealed = true;
					BlankReveal(*tile.adjacentTiles[i]);
				}
			}
			else if (!tile.adjacentTiles[i]->mine && !tile.adjacentTiles[i]->flag)
			{
				if (!tile.adjacentTiles[i]->revealed)
				{
					tile.adjacentTiles[i]->revealed = true;
				}
			}
		}
	}
}
void Board::UpdateCounter(std::vector<sf::Texture>& digits, sf::RenderWindow& window)
{
	int count = mines;
	int num;
	float y = (float)height - 88;
	float x = 21.0f;
	if (count < 0)
	{
		sf::Sprite negative(digits[10]);
		negative.setPosition(x - 21, y);
		window.draw(negative);
	}
	num = count / 100;
	sf::Sprite Hun(digits[num]);
	Hun.setPosition(x, y);
	window.draw(Hun);
	num = count / 10;
	num = num % 10;
	sf::Sprite Ten(digits[abs(num)]);
	Ten.setPosition(x + 21, y);
	window.draw(Ten);
	num = count % 10;
	sf::Sprite One(digits[abs(num)]);
	One.setPosition(x + 42, y);
	window.draw(One);
}
void LoadDigits(std::vector<sf::Texture>& digits)
{
	
	for (int i = 0; i < 11; ++i)
	{
		sf::Texture texture;
		texture.loadFromFile("images/digits.png", sf::IntRect(21 * i, 0, 21, 32));
		digits.push_back(texture);
	}
}
int Board::GetHeight()
{
	return height;
}
bool Board::GetGameOver()
{
	return gameOver;
}
bool Board::GetSmiley()
{
	return smiley;
}
bool Board::GetTest1()
{
	return test1;
}
bool Board::GetTest2()
{
	return test2;
}
bool Board::GetTest3()
{
	return test3;
}
void Board::CheckWin()
{
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			if ((tiles[i][j].blank || tiles[i][j].numAdjacent > 0) && !tiles[i][j].revealed && !tiles[i][j].mine)
			{
				return;
			}
		}
	}
	win = true;
	mines = 0;
	gameOver = true;
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			if (tiles[i][j].mine)
			{
				tiles[i][j].flag = true;
			}
		}
	}
}

void Board::LoadMines(std::ifstream& file)
{
	string line;
	
	int row = 0;
	while (!file.eof())
	{
		getline(file, line);
		for (unsigned int i = 0; i < line.size(); i++)
		{
			if (line[i] == '1')
			{
				tiles[i][row].mine = true;
				tiles[i][row].blank = false;
			}
		}
		row++;
	}
}
Board LoadBoard(std::ifstream& file)
{
	string line;
	int rows = 0;
	int mines = 0;
	int columns;
	while (!file.eof())
	{
		getline(file, line);
		for (unsigned int i = 0; i < line.size(); i++)
		{
			if (line[i] == '1')
			{
				mines++;
			}
		}
		rows++;
	}
	columns = line.size();
	int numTiles = rows * columns;
	int width = columns * 32;
	int height = (rows * 32) + 88;
	Board board(columns, rows, mines, numTiles, width, height);
	return board;
}