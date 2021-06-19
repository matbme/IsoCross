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
    // Compiler: Complains 'static' is not permitted on type declaration
    // Compiler: Compiles anyway
    static enum TileTexture {
        nothing = 99,
        // ground types
        spiky_grass = 48,
        grass = 49,
        stone = 50,
        dirt = 51,
        water = 56, //separate file
        // fences
        grassy_fence_back_right = 0,
        fence_back_right = 1,
        grassy_fence_back_left = 2,
        fence_back_left = 3,
        grassy_fence_front_right = 4,
        fence_front_right = 5,
        grassy_fence_front_left = 6,
        fence_front_left = 7,
        grassy_fence_corner_down = 8,
        fence_corner_down = 9,
        grassy_fence_corner_up = 10,
        fence_corner_up = 11,
        grassy_fence_corner_right = 12,
        fence_corner_right = 13,
        grassy_fence_corner_left = 14,
        fence_corner_left = 15,
    };

    Tile(TileTexture texMap);
    ~Tile();

    void initialize(TileTexture texMap);
};

#endif
