#pragma once

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define FPS 120

#define CELL_SIZE 60

#define DEFAULT_PLAYER_SPEED 9

#define GAMEBOARD_WIDTH 25
#define GAMEBOARD_HEIGHT 15

#define GRID_LINE_WIDTH 5
#define DIRECTION_LINE_WIDTH 5

#define ROOM_SIZE 27
#define MAP_SIZE 5

#define MINIMAP_SIZE 14
#define MINIMAP_GAP 6
#define MINIMAP_CELL_SIZE 20
#define MINIMAP_X WINDOW_WIDTH - 120
#define MINIMAP_Y 20

#define CURR_ROOM_INDICATOR_SIZE 8

#include "Direction.h"

using namespace std;

extern int dr[DIRECTION_COUNT];
extern int dc[DIRECTION_COUNT];