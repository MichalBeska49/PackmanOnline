#ifndef MAZE_H
#define MAZE_H

#include "game/entities/pacman.h"
#include "game/resources.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QVector>

class Maze: public QObject {
    Q_OBJECT
public:
    explicit Maze();

    uint getTile(int x, int y) const;
    void renderMaze(QGraphicsScene *scene);
    bool removeDot(Pacman* pacman);
    bool tileBlocks(int x, int y);
    bool isIntersection(int x, int y);

signals:
    void bigDotIsEaten();

private:
    uint tiles[Resources::MAZE_SIZE][Resources::MAZE_SIZE];

    static constexpr int EMPTY_FIELD = 0;
    static constexpr int WALL_FIELD = 1;
    static constexpr int SMALL_DOT_FIELD = 2;
    static constexpr int BIG_DOT_FIELD = 3;
    static constexpr int DOOR_FIELD = 4;
};

#endif // MAZE_H
