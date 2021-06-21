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
        // Selector highlight
        selector = 57, // separate file
        // Characters
        enemy_idle = 58,
        enemy_go_left = 59,
        enemy_go_down = 60,
        enemy_go_right = 62,
        enemy_go_up = 61,
        enemy_walk_left_start = 63,
        enemy_walk_left_mid = 64,
        enemy_walk_left_end = 65,
        enemy_walk_right_start = 67,
        enemy_walk_right_mid = 68,
        enemy_walk_right_end = 69,
        player_idle = 72,
        player_go_left = 81,
        player_go_down = 78,
        player_go_right = 76,
        player_go_up = 80,
        player_walk_left_start = 77,
        player_walk_left_mid = 78,
        player_walk_left_end = 79,
        player_walk_right_start = 81,
        player_walk_right_mid = 82,
        player_walk_right_end = 83,
        player_hit = 84,
        player_dead = 85,
    };

    TileTexture texMap;

    Tile(TileTexture texMap);
    ~Tile() {}

    void initialize(TileTexture texMap);
};

#endif
