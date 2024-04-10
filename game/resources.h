#ifndef RESOURCES_H
#define RESOURCES_H
#include <QString>
#include <QPoint>
#include <QSize>
#include <QColor>
#include <QUrl>

/**
 * @class Resources
 * @brief Klasa zawierająca zasoby i definicje używane w grze.
 *
 * Klasa Resources definiuje zestaw wspólnych zasobów i stałych używanych w grze,
 * takich jak kierunki ruchu, typy duchów, rozmiary elementów gry, oraz typy wiadomości
 * komunikacji między klientem a serwerem. Nie zawiera instancji obiektów ani metod niestatycznych.
 */
class Resources
{
public:
    Resources();

    enum class Direction
    {
        Up = 1,
        Down = -1,
        Left = 2,
        Right = -2,
        Unset = 0
    };

    enum class GhostType
    {
        Blinky = 0,
        Clyde = 1,
        Inky = 2,
        Pinky = 3
    };

    enum class ClientMessageType {
        Pacmans = 0,
        Ghosts = 1,
        ClientPacmanId = 2,
        PacmanEatDot = 3
    };

    enum class HostMessageType {
        PacmanChangeDirection = 0
    };

    constexpr static const unsigned int MAZE_TILE_SIZE = 20;
    constexpr static const unsigned int MAZE_SIZE = 19;

    constexpr static const unsigned int DOT_SIZE = 4;
    constexpr static const unsigned int BIG_DOT_SIZE = 6;

    constexpr static int FPS = 30;
};

#endif // RESOURCES_H

