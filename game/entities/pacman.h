#ifndef PACMAN_H
#define PACMAN_H

#include "game/entities/character.h"
#include "game/resources.h"
#include <QPainter>
#include <QKeyEvent>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

/**
 * @class Pacman
 * @brief Klasa reprezentująca postać Pacmana w grze, dziedzicząca po QObject oraz Character.
 *
 * Klasa Pacman zarządza wszystkimi aspektami dotyczącymi postaci Pacmana, takimi jak ruch,
 * kierunek, punktacja, stan (żywy lub martwy) oraz interakcje z elementami gry, takimi jak kropki (dots).
 */
class Pacman :public QObject, public Character {
public:
    /**
     * @brief Konstruktor tworzący obiekt Pacmana z określonym identyfikatorem i nazwą.
     * @param id Unikalny identyfikator dla obiektu Pacmana.
     * @param name Nazwa przypisana do obiektu Pacmana.
     */
    Pacman(int id, QString name);
    /**
     * @brief Konstruktor domyślny.
     */
    Pacman();

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

    QString getName();

    QJsonObject toJsonObject() const;
    void fromJsonObject(const QJsonObject &json);
    void reaload(Pacman* newPacman);

private:
    int id;
    bool dead;
    int eatenDots;
    int score;
    QString name;
    Resources::Direction direction;
    Resources::Direction nextDirection;
};


#endif // PACMAN_H
