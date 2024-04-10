#ifndef MAZE_H
#define MAZE_H

#include "game/entities/pacman.h"
#include "game/resources.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QVector>

/**
 * @class Maze
 * @brief Klasa reprezentująca labirynt w grze, dziedzicząca po QObject.
 *
 * Klasa Maze służy do zarządzania strukturą labiryntu w grze, w tym umieszczaniem kropek (dots) do zbierania przez Pacmana,
 * renderowaniem labiryntu, oraz sprawdzaniem kolizji i interakcji z Pacmanem oraz duchami.
 */
class Maze: public QObject {
    Q_OBJECT
public:
    /**
     * @brief Konstruktor klasy Maze.
     */
    explicit Maze();

    uint getTile(int x, int y) const;
    void renderMaze(QGraphicsScene *scene);
    bool removeDot(Pacman* pacman);
    void removeDot(int x, int y);
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
