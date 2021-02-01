#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include "Cell.h"

#define POLE 49

enum Player
{
    FROM_FOX = 1,
    FROM_CHICKEN
};

class Interface
{
    sf::RenderWindow* window;
    Cell *arr[POLE];
    
    Player type;

    sf::Sprite background;
    sf::Texture tBackground;
    sf::Sprite fox;
    sf::Texture tFox;
    sf::Sprite chicken;
    sf::Texture tChicken;
    
public:
    void OpenApp();
    void IsReady();
    void CreateArea();
    int CheckFoxMove(int col, int row, vector<Cell*> &ptr);
    int CheckWin();

    void render();
    void drawBlock(int x, int y, int size);
    ~Interface() {
    }
};