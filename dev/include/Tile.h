#ifndef TILE_H
#define TILE_H

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "Sprite.h"

class Tile : public Sprite {
public:
    static enum TileTexture {
        spiky_grass = 48,
        grass = 49,
    };

    Tile(TileTexture texMap);
    ~Tile();

    void initialize(TileTexture texMap);
};

#endif
