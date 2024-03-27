#include "game/entities/pacman.h"

Pacman::Pacman(int id): Character(9, 11),
    id(id),
    eatenDots(0),
    dead(false),
    direction(Resources::Direction::Unset){}

QRectF Pacman::boundingRect() const {
    return QRectF(0, 0, 20, 20);
}

void Pacman::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(boundingRect());
}

Resources::Direction Pacman::getDirection()
{
    return direction;
}

Resources::Direction Pacman::getNextDirection()
{
    return nextDirection;
}

void Pacman::setNextDirection(Resources::Direction d) {
    if(d == Resources::Direction::Unset){
        direction = d;
        return;

    } else if (nextDirection == d) {
        return;
    }

    nextDirection = d;
}

void Pacman::setDirection(Resources::Direction d) {

    direction = d;
}

int Pacman::getId()
{
    return id;
}


void Pacman::eatDot()
{
    eatenDots++;
}

int Pacman::getDotsEaten()
{
    return eatenDots;
}

void Pacman::setDead(bool d)
{
    dead = d;
}

bool Pacman::isDead()
{
    return dead;
}

int Pacman::getScore(){
    return score;
}

void Pacman::setScore(int s){
    score = s;
}

void Pacman::move()
{
        switch (direction)
        {
        case Resources::Direction::Up:
            Character::move(0, -1);
            break;
        case Resources::Direction::Down:
            Character::move(0, 1);
            break;
        case Resources::Direction::Left:
            Character::move(-1, 0);
            break;
        case Resources::Direction::Right:
            Character::move(1, 0);
            break;
        case Resources::Direction::Unset:

            break;
        }
}
