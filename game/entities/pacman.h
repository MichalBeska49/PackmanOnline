#ifndef PACMAN_H
#define PACMAN_H

#include "game/entities/character.h"
#include "game/resources.h"
#include <QPainter>
#include <QKeyEvent>

class Pacman :public QObject, public Character {
public:
    Pacman(int id);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void move();

    Resources::Direction getDirection();
    Resources::Direction getNextDirection();
    void setDirection(Resources::Direction direction);
    void setNextDirection(Resources::Direction direction);
    void stop();

    void eatDot();
    int getDotsEaten();

    void setDead(bool d);
    bool isDead();
    int getId();

    int getScore();
    void setScore(int s);

private:
    int id;
    bool dead;
    int eatenDots;
    int score;
    Resources::Direction direction;
    Resources::Direction nextDirection;
};


#endif // PACMAN_H
