#include "Maze.h"
#include "game/entities/pacman.h"

Maze::Maze(): tiles{
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 3, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 3, 1},
        {1, 2, 1, 1, 0, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
        {1, 2, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 2, 2, 2, 1},
        {1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 1, 2, 1, 1, 4, 1, 1, 2, 1, 2, 1, 1, 1, 1},
        {0, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 0},
        {1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1},
        {1, 1, 1, 1, 2, 1, 2, 0, 0, 0, 0, 0, 2, 1, 2, 1, 1, 1, 1},
        {1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 1},
        {1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1},
        {1, 2, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 2, 1},
        {1, 3, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 3, 1},
        {1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1},
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    } {}

void Maze::renderMaze(QGraphicsScene *scene) {
    for (int i = 0; i < (int)Resources::MAZE_SIZE; ++i) {
        for (int j = 0; j < (int)Resources::MAZE_SIZE; ++j) {
            if (tiles[i][j] == EMPTY_FIELD) {
                QGraphicsRectItem *empty = new QGraphicsRectItem(j * Resources::MAZE_TILE_SIZE, i * Resources::MAZE_TILE_SIZE, Resources::MAZE_TILE_SIZE, Resources::MAZE_TILE_SIZE);
                empty->setBrush(Qt::black);
                scene->addItem(empty);
            } else if (tiles[i][j] == WALL_FIELD) {
                QGraphicsRectItem *wall = new QGraphicsRectItem(j * Resources::MAZE_TILE_SIZE, i * Resources::MAZE_TILE_SIZE, Resources::MAZE_TILE_SIZE, Resources::MAZE_TILE_SIZE);
                wall->boundingRect();
                wall->setBrush(Qt::blue);
                scene->addItem(wall);
            } else if (tiles[i][j] == SMALL_DOT_FIELD) {
                QGraphicsRectItem *dot = new QGraphicsRectItem(j * Resources::MAZE_TILE_SIZE + Resources::MAZE_TILE_SIZE/2 - Resources::DOT_SIZE/2, i * Resources::MAZE_TILE_SIZE + Resources::MAZE_TILE_SIZE/2 - Resources::DOT_SIZE/2, Resources::DOT_SIZE, Resources::DOT_SIZE);
                dot->boundingRect();
                dot->setBrush(Qt::white);
                scene->addItem(dot);
            } else if (tiles[i][j] == BIG_DOT_FIELD) {
                QGraphicsRectItem *bigDot = new QGraphicsRectItem(j * Resources::MAZE_TILE_SIZE + Resources::MAZE_TILE_SIZE/2 - Resources::BIG_DOT_SIZE/2, i * Resources::MAZE_TILE_SIZE + Resources::MAZE_TILE_SIZE/2 - Resources::BIG_DOT_SIZE/2, Resources::BIG_DOT_SIZE, Resources::BIG_DOT_SIZE);
                bigDot->boundingRect();
                bigDot->setBrush(Qt::white);
                scene->addItem(bigDot);
            } else if (tiles[i][j] == DOOR_FIELD) {
                QGraphicsRectItem *door = new QGraphicsRectItem(j * Resources::MAZE_TILE_SIZE, i * Resources::MAZE_TILE_SIZE, Resources::MAZE_TILE_SIZE, Resources::MAZE_TILE_SIZE);
                door->boundingRect();
                door->setBrush(QColorConstants::Svg::purple);
                scene->addItem(door);
            }

        }
    }
}

bool Maze::tileBlocks(int x, int y)
{
    return tiles[y][x] == WALL_FIELD || tiles[y][x] == DOOR_FIELD;
}

bool Maze::removeDot(Pacman* pacman)
{
    if (tiles[pacman->getTileY()][pacman->getTileX()] == SMALL_DOT_FIELD)
    {
        tiles[pacman->getTileY()][pacman->getTileX()] = EMPTY_FIELD;
        pacman->eatDot();
        pacman->setScore(pacman->getScore() + 5);
        return true;
    }
    else if (tiles[pacman->getTileY()][pacman->getTileX()] == BIG_DOT_FIELD)
    {
        tiles[pacman->getTileY()][pacman->getTileX()] = EMPTY_FIELD;
        pacman->setScore(pacman->getScore() + 20);
        emit bigDotIsEaten();
        return true;
    }
    return false;
}

bool Maze::isIntersection(int x, int y){
    int emptyCount = 0;

    if(tiles[y-1][x] != WALL_FIELD && tiles[y-1][x] != DOOR_FIELD ) ++emptyCount;
    if(tiles[y+1][x] != WALL_FIELD && tiles[y+1][x] != DOOR_FIELD ) ++emptyCount;
    if(tiles[y][x-1] != WALL_FIELD && tiles[y][x-1] != DOOR_FIELD ) ++emptyCount;
    if(tiles[y][x+1] != WALL_FIELD && tiles[y-1][x+1] != DOOR_FIELD ) ++emptyCount;

    return emptyCount >= 3;
}

uint Maze::getTile(int x, int y) const
{
    return tiles[x][y];
}
