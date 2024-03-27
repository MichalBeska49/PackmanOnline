#include "game/entities/ghost.h"

Ghost::Ghost(int tilePosX, int tilePosY, int destinationX, int destinationY, Resources::Direction moving, Resources::GhostType ghostType)
    : Character(tilePosX, tilePosY)
    , scattering(true)
    , outOfCage(false)
    , ghostType(ghostType)
    , speed(1)
    , moving(moving)
    , weak(false)
{
    setDestination(destinationX, destinationY);
}

void Ghost::setDestination(int x, int y)
{
    destinationTileX = x;
    destinationTileY = y;
}

int Ghost::getDestX()
{
    return destinationTileX;
}

int Ghost::getDestY()
{
    return destinationTileY;
}

Resources::Direction Ghost::getMoving()
{
    return moving;
}

void Ghost::setMoving(Resources::Direction d)
{
    moving = d;
}

void Ghost::setSpeed(float s){
    speed = s;
}

Resources::GhostType Ghost::getGhostType()
{
    return ghostType;
}

void Ghost::move()
{
    switch (moving)
    {
    case Resources::Direction::Up:
        Character::move(0, -speed);
        break;
    case Resources::Direction::Down:
        Character::move(0, speed);
        break;
    case Resources::Direction::Left:
        Character::move(-speed, 0);
        break;
    case Resources::Direction::Right:
        Character::move(speed, 0);
        break;
    }
}

bool Ghost::isScattering()
{
    return scattering;
}

void Ghost::setScattering(bool s)
{
    scattering = s;
}

bool Ghost::isOutOfCage()
{
    return outOfCage;
}

void Ghost::laveCage()
{
    outOfCage = true;
}

bool Ghost::isWeak()
{
    return weak;
}

void Ghost::setWeak(bool w)
{
    weak = w;
}

bool Ghost::getChangeMoving(){
    return changeMoving;
}

void Ghost::setChangeMoving(bool d){
    changeMoving = d;
}

QRectF Ghost::boundingRect() const {
    return QRectF(0, 0, 20, 20);
}

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(weak){
        painter->setBrush(Qt::blue);
    } else {
        switch (ghostType)
        {
        case Resources::GhostType::Blinky:
            painter->setBrush(Qt::red);
            break;
        case Resources::GhostType::Clyde:
            painter->setBrush(QColorConstants::Svg::orange);
            break;
        case Resources::GhostType::Inky:
            painter->setBrush(Qt::cyan);
            break;
        case Resources::GhostType::Pinky:
            painter->setBrush(QColorConstants::Svg::pink);
            break;
        }
    }


    painter->drawRect(boundingRect());
}

Pacman* Ghost::findClosetPacman(QList<Pacman*> pacmans) {
    Pacman* closetPacman;
    float min = Resources::MAZE_SIZE;

    for (Pacman* pacman : pacmans) {
        float distance = std::sqrt(std::pow(pacman->getTileX() - Character::getTileX(), 2) + std::pow(pacman->getTileY() - Character::getTileY(), 2));

        if (distance < min) {
            min = distance;
            closetPacman = pacman;
        }
    }

    return closetPacman;
}

bool Ghost::shouldTakeDecision()
{
    return decision;
}

void Ghost::setTakeDecision(bool d)
{
    decision = d;
}

