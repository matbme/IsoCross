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

    // try left
    if (bottomMap[pos - 1] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::right) {
        std::cout << "left" << std::endl;
        topMap[pos - 1] = Tile::TileTexture::player_go_left;
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::left);
    }
    // try up
    else if (bottomMap[pos - xSize] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::down) {
        std::cout << "up" << std::endl;
        topMap[pos - xSize] = Tile::TileTexture::player_go_up;
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::up);
    }
    // try right
    else if (bottomMap[pos + 1] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::left) {
        std::cout << "right" << std::endl;
        topMap[pos + 1] = Tile::TileTexture::player_go_right;
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::right);
    }
    // try down
    else if (bottomMap[pos + xSize] == Tile::TileTexture::stone && Character::getLastMove() != Character::Movement::up) {
        std::cout << "down" << std::endl;
        topMap[pos + xSize] = Tile::TileTexture::player_go_down;
        topMap[pos] = Tile::TileTexture::nothing;
        Character::setLastMove(Character::Movement::down);
    }
}

void Character::enemyPatrol(int *bottomMap, int *topMap, int position, int xSize, int ySize) {
    int pos = position - 1;

    // edges detection
    if (topMap[pos] == Tile::TileTexture::enemy_go_up && pos - xSize < 0) //top
        topMap[pos] = Tile::TileTexture::enemy_go_down;
    else if (topMap[pos] == Tile::TileTexture::enemy_go_left && pos % xSize == 0) // left
        topMap[pos] = Tile::TileTexture::enemy_go_right;
    else if (topMap[pos] == Tile::TileTexture::enemy_go_right && (pos + 1) % xSize == 0) // right
        topMap[pos] = Tile::TileTexture::enemy_go_left;
    else if (topMap[pos] == Tile::TileTexture::enemy_go_down && pos + xSize > xSize*ySize - 1) // down
        topMap[pos] = Tile::TileTexture::enemy_go_up;

    // enemy patrolling to the left
    if (topMap[pos] == Tile::TileTexture::enemy_go_left) {
        topMap[pos - 1] = Tile::TileTexture::enemy_go_left;
        topMap[pos] = Tile::TileTexture::nothing;
    }
    // enemy patrolling up
    if (topMap[pos] == Tile::TileTexture::enemy_go_up) {
        topMap[pos - xSize] = Tile::TileTexture::enemy_go_up;
        topMap[pos] = Tile::TileTexture::nothing;
    }
    // enemy patrolling to the right
    if (topMap[pos] == Tile::TileTexture::enemy_go_right) {
        topMap[pos + 1] = Tile::TileTexture::enemy_go_right;
        topMap[pos] = Tile::TileTexture::nothing;
    }
    // enemy patrolling down
    if (topMap[pos] == Tile::TileTexture::enemy_go_down) {
        topMap[pos + xSize] = Tile::TileTexture::enemy_go_down;
        topMap[pos] = Tile::TileTexture::nothing;
    }
}
