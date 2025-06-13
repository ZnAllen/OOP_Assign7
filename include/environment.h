#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include "unit.h"

constexpr int GAME_WINDOW_WIDTH = 50;
constexpr int GAME_WINDOW_HEIGHT = 39;

constexpr Color PLAYER1 = BLUE;
constexpr Color PLAYER2 = PINK;

constexpr Color BackgroundColor = GREEN;
static int P1BackgroundColor = 117;
static int P2BackgroundColor = 229;

constexpr int P1ForegroundColor = 21;
constexpr int P2ForegroundColor = 91;

constexpr int GAME_WINDOW_CELL_WIDTH = 2;
constexpr int WINDOW_PIXEL_WIDTH = GAME_WINDOW_WIDTH * GAME_WINDOW_CELL_WIDTH;
constexpr int WINDOW_PIXEL_HEIGHT = GAME_WINDOW_HEIGHT; 

constexpr double SPF = 0.1; // Second Per Frame 
#endif
