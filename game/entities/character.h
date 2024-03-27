#ifndef CHARACTER_H
#define CHARACTER_H

#include <QGraphicsItem>

class Character : public QGraphicsItem {
public:
    Character(int tilePosX, int tilePosY);

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
