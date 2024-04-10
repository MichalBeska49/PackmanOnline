#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsItem>

/**
 * @class Character
 * @brief Klasa reprezentująca postać w grze, dziedzicząca po QGraphicsItem.
 *
 * Klasa Character służy do reprezentowania postaci w grze. Pozwala na zarządzanie
 * pozycją postaci na ekranie oraz na planszy gry. Umożliwia ruch oraz teleportację postaci.
 * Jest to klasa abstrakcyjna, wymagająca implementacji metod boundingRect i paint w klasach pochodnych.
 */
class Character : public QGraphicsItem {
public:
    /**
     * @brief Konstruktor tworzący postać na określonej pozycji na planszy.
     * @param tilePosX Pozycja X postaci na planszy.
     * @param tilePosY Pozycja Y postaci na planszy.
     */
    Character(int tilePosX, int tilePosY);
    /**
     * @brief Konstruktor domyślny.
     */
    Character();

    float getScreenPosX();
    float getScreenPosY();
    int getTileX();
    int getTileY();

    void move(float x, float y);
    void teleport(int x, int y);

    virtual QRectF boundingRect() const override = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override = 0;

protected:
    float screenPosX;
    float screenPosY;
    int tileX;
    int tileY;
};

#endif // CHARACTER_H
