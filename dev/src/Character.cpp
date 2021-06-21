#include "Character.h"

Character::Movement Character::playerLastMove = Character::Movement::none;

Character::Character(Tile::TileTexture texMap) : Tile(texMap) {}

Character::~Character() {}

Character::Movement Character::getLastMove() {
    return Character::playerLastMove;
}

void Character::setLastMove(Character::Movement move) {
    Character::playerLastMove = move;
}

void Character::followPath(int* bottomMap, int* topMap, int position, int xSize) {
    int pos = position - 1;
    std::cout << "pos: " << pos << std::endl;
    std::cout << "last move " << Character::getLastMove() << std::endl;
    // try left
    if (bottomMap[pos - 1] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::right) {
        std::cout << "left" << std::endl;
        topMap[pos - 1] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::left);
    }
    // try up
    else if (bottomMap[pos - xSize] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::down) {
        std::cout << "up" << std::endl;
        topMap[pos - xSize] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::up);
    }
    // try right
    else if (bottomMap[pos + 1] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::left) {
        std::cout << "right" << std::endl;
        topMap[pos + 1] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::right);
    }
    // try down
    else if (bottomMap[pos + xSize] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::up) {
        std::cout << "down" << std::endl;
        topMap[pos + xSize] = topMap[pos];
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::down);
    }
}
