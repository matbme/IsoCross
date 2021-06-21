#include "Character.h"

Character::Character(Tile::TileTexture texMap) : Tile(texMap) {}
Character::~Character() {}

void Character::followPath(int* bottomMap, int* topMap, int position, int xSize) {
    int pos = position - 1;
    std::cout << pos << std::endl;
    // try left
    if (bottomMap[pos - 1] == Tile::TileTexture::stone) {
        std::cout << "left" << std::endl;
        topMap[pos - 1] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
    }
    // try up
    else if (bottomMap[pos - xSize] == Tile::TileTexture::stone) {
        std::cout << "up" << std::endl;
        topMap[pos - xSize] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
    }
    // try right
    else if (bottomMap[pos + 1] == Tile::TileTexture::stone) {
        std::cout << "right" << std::endl;
        topMap[pos + 1] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
    }
    // try down
    else if (bottomMap[pos + xSize] == Tile::TileTexture::stone) {
        std::cout << "down" << std::endl;
        topMap[pos + xSize] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
    }
}
