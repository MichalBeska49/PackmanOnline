#ifndef GHOST_H
#define GHOST_H

#include "game/entities/character.h"
#include "game/entities/pacman.h"
#include "game/resources.h"
#include <QPainter>

/**
 * @class Ghost
 * @brief Klasa reprezentująca ducha w grze, dziedzicząca po QObject oraz Character.
 *
 * Klasa Ghost służy do zarządzania duchami w grze, umożliwiając kontrolę ich ruchu, stanu oraz interakcji z innymi obiektami gry.
 * Duchy mogą poruszać się w określonych kierunkach, ścigać Pacmana, uciekać, kiedy są słabe, oraz wracać do klatki po zostaniu zjedzonym.
 */
class Ghost :public QObject, public Character {
public:
    /**
     * @brief Konstruktor tworzący ducha z określonymi parametrami początkowymi.
     * @param tilePosX Pozycja początkowa X.
     * @param tilePosY Pozycja początkowa Y.
     * @param destinationX Pozycja docelowa X.
     * @param destinationY Pozycja docelowa Y.
     * @param moving Początkowy kierunek ruchu ducha.
     * @param ghostType Typ ducha.
     */
    Ghost(int tilePosX, int tilePosY, int destinationX, int destinationY, Resources::Direction moving, Resources::GhostType ghostType);
    /**
     * @brief Konstruktor domyślny.
     */
    Ghost();

    void setDestination(int x, int y);
    int getDestX();
    int getDestY();
    bool getChangeMoving();
    void setChangeMoving(bool d);

    void setDirection(Resources::Direction d);
    Resources::Direction getDirection();
    void move();

    bool isWeak();
    void setWeak(bool weak);

    bool isOutOfCage();

    bool isScattering();
    void setScattering(bool s);

    void laveCage();
    Pacman* findClosetPacman(std::list<Pacman*> pacmans);

    bool shouldTakeDecision();
    void setTakeDecision(bool d);

    void setSpeed(float speed);

    Resources::GhostType getGhostType();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QJsonObject toJsonObject() const;
    void fromJsonObject(const QJsonObject &json);
    void reaload(Ghost* ghost);

private:
    int destinationTileX;
    int destinationTileY;
    Resources::Direction direction;
    Resources::GhostType ghostType;

    bool decision;
    bool changeDirection;
    bool weak;

    bool scattering;
    bool outOfCage;
    float speed;
};

#endif // GHOST_H
