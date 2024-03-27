#ifndef GHOST_H
#define GHOST_H

#include "game/entities/character.h"
#include "game/entities/pacman.h"
#include "game/resources.h"
#include <QPainter>

class Ghost :public QObject, public Character {
public:
    Ghost(int tilePosX, int tilePosY, int destinationX, int destinationY, Resources::Direction moving, Resources::GhostType ghostType);

    void setDestination(int x, int y);
    int getDestX();
    int getDestY();
    bool getChangeMoving();
    void setChangeMoving(bool d);

    void setMoving(Resources::Direction d);
    Resources::Direction getMoving();
    void move();

    bool isWeak();
    void setWeak(bool weak);

    bool isOutOfCage();

    bool isScattering();
    void setScattering(bool s);

    void laveCage();
    Pacman* findClosetPacman(QList<Pacman*> pacmans);

    bool shouldTakeDecision();
    void setTakeDecision(bool d);

    void setSpeed(float speed);

    Resources::GhostType getGhostType();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int destinationTileX;
    int destinationTileY;
    Resources::Direction moving;
    Resources::GhostType ghostType;

    bool decision;
    bool changeMoving;
    bool weak;

    bool scattering;
    bool outOfCage;
    float speed;
};

#endif // GHOST_H
