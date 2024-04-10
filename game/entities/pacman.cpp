#include "game/entities/pacman.h"

Pacman::Pacman(int id, QString name): Character(9, 11),
    id(id),
    eatenDots(0),
    dead(false),
    name(name),
    direction(Resources::Direction::Unset)
{}

Pacman::Pacman(): Character(){

}

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

QString Pacman::getName()
{
    return name;
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

QJsonObject Pacman::toJsonObject() const {
    QJsonObject json;

    json["id"] = id;
    json["dead"] = dead;
    json["eatenDots"] = eatenDots;
    json["score"] = score;
    json["name"] = name;
    json["direction"] = int(direction);
    json["nextDirection"] = int(nextDirection);
    json["screenPosX"] = screenPosX;
    json["screenPosY"] = screenPosY;
    json["tileX"] = tileX;
    json["tileY"] = tileY;
    return json;
}

void Pacman::fromJsonObject(const QJsonObject &json) {
    id = json["id"].toInt();
    dead = json["dead"].toBool();
    eatenDots = json["eatenDots"].toInt();
    score = json["score"].toInt();
    name = json["name"].toString();
    direction = static_cast<Resources::Direction>(json["direction"].toInt());
    nextDirection = static_cast<Resources::Direction>(json["nextDirection"].toInt());
    screenPosX = json["screenPosX"].toInt();
    screenPosY = json["screenPosY"].toInt();
    tileX = json["tileX"].toInt();
    tileY = json["tileY"].toInt();
}

void Pacman::reaload(Pacman* newPacman) {
    dead = newPacman->dead;
    eatenDots = newPacman->eatenDots;
    score = newPacman->score;
    name = newPacman->name;
    direction = newPacman->direction;
    nextDirection = newPacman->nextDirection;
    screenPosX = newPacman->screenPosX;
    screenPosY = newPacman->screenPosY;
    tileX = newPacman->tileX;
    tileY = newPacman->tileY;
}
