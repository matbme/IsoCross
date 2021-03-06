#ifndef CHARACTER_H
#define CHARACTER_H

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Sprite.h"
#include "Tile.h"

class Character : public Tile {
public:
    Character(Tile::TileTexture texMap);
    ~Character();

    static enum Movement {
        none = -1,
        left = 0,
        right = 1,
        up = 2,
        down = 3,
    };

    static enum NextState {
        stop = 0,
        move = 1,
        dead = 2,
        win = 3,
    };

    Movement getLastMove();
    void setLastMove(Character::Movement move);

    NextState followPath(int* bottomMap, int* topMap, int position, int xSize);
    void enemyPatrol(int *bottomMap, int *topMap, int position, int xSize, int ySize);

private:
    static Movement playerLastMove;
};

#endif
