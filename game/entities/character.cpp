#include "game/entities/character.h"
#include <cmath>
#include "game/resources.h"

Character::Character(int tilePosX, int tilePosY)
{
    tileX = tilePosX;
    tileY = tilePosY;
    screenPosX = tilePosX * Resources::MAZE_TILE_SIZE;
    screenPosY = tilePosY * Resources::MAZE_TILE_SIZE;
}

Character::Character() {}

float Character::getScreenPosX()
{
    return screenPosX;
}

float Character::getScreenPosY()
{
    return screenPosY;
}

int Character::getTileX()
{
    return tileX;
}

int Character::getTileY()
{
    return tileY;
}

void Character::move(float x, float y)
{
    screenPosX += x;
    screenPosY += y;

    if ((int)(screenPosX) % Resources::MAZE_TILE_SIZE == 0 && (int)(screenPosY) % Resources::MAZE_TILE_SIZE == 0)
    {
        tileX = (int) round(screenPosX / Resources::MAZE_TILE_SIZE);
        tileY = (int) round(screenPosY / Resources::MAZE_TILE_SIZE);
    }
}

void Character::teleport(int x, int y)
{
    tileX = x;
    tileY = y;
    screenPosX = x * Resources::MAZE_TILE_SIZE;
    screenPosY = y * Resources::MAZE_TILE_SIZE;
}
