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

    void followPath(int* bottomMap, int* topMap, int position, int xSize);
};

#endif
