#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include "extrafile.h"
using namespace std;
int main()
{
    int width;
    int height;
    int columns;
    int rows;
    int mines;
    int numTiles;
    std::vector<sf::Texture> digits;
    std::ifstream file;
    file.open("boards/config.cfg");
    file >> columns;
    file >> rows;
    file >> mines;
    file.close();
    numTiles = rows * columns;
    width = columns * 32;
    height = (rows * 32) + 88;
    Board board(columns, rows, mines, numTiles, width, height);
    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper");
    LoadDigits(digits);
    board.PlaceMines();
    board.PlaceAdjacent();
    board.DrawBoard(window);
    board.UpdateCounter(digits, window);
    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                auto mousePosition = sf::Mouse::getPosition(window);
                int x = (int)mousePosition.x;
                int y = (int)mousePosition.y;
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (y < board.GetHeight() - 88 && !board.GetGameOver())
                    {
                        board.BoardReveal(x, y);
                        board.CheckWin();
                        board.DrawBoard(window);
                        board.UpdateCounter(digits, window);
                        window.display();
                    }
                    if (y >= board.GetHeight() - 88 && y <= board.GetHeight() - 24)
                    {
                        board.ButtonPress(x, y);
                        if (board.GetSmiley())
                        {
                            Board temp(columns, rows, mines, numTiles, width, height);
                            board = temp;
                            board.PlaceMines();
                            board.PlaceAdjacent();
                            board.DrawBoard(window);
                            board.UpdateCounter(digits, window);
                            window.display();
                        }
                        else if (board.GetTest1())
                        {
                            
                            file.open("boards/testboard1.brd");
                            board = LoadBoard(file);
                            file.clear();
                            file.seekg(0);
                            board.LoadMines(file);
                            file.close();
                            board.PlaceAdjacent();
                            board.DrawBoard(window);
                            board.UpdateCounter(digits, window);
                            window.display();
                        }
                        else if (board.GetTest2())
                        {
                            
                            file.open("boards/testboard2.brd");
                            board = LoadBoard(file);
                            file.clear();
                            file.seekg(0);
                            board.LoadMines(file);
                            file.close();
                            board.PlaceAdjacent();
                            board.DrawBoard(window);
                            board.UpdateCounter(digits, window);
                            window.display();
                        }
                        else if (board.GetTest3())
                        {
                            
                            file.open("boards/testboard3.brd");
                            board = LoadBoard(file);
                            file.clear();
                            file.seekg(0);
                            board.LoadMines(file);
                            file.close();
                            board.PlaceAdjacent();
                            board.DrawBoard(window);
                            board.UpdateCounter(digits, window);
                            window.display();
                        }
                        else
                        {
                            board.DrawBoard(window);
                            board.UpdateCounter(digits, window);
                            window.display();
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (y < board.GetHeight() - 88 && !board.GetGameOver())
                    {
                        board.PlaceFlag(x, y);
                        board.DrawBoard(window);
                        board.UpdateCounter(digits, window);
                        window.display();
                    }
                }
            }
        }
    }
    return 0;
}