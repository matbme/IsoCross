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

    Movement getLastMove();
    void setLastMove(Character::Movement move);

    void followPath(int* bottomMap, int* topMap, int position, int xSize);

private:
    static Movement playerLastMove;
};

#endif
