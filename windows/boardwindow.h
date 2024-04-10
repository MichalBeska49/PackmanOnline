#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "game/entities/pacman.h"
#include "game/entities/ghost.h"
#include "game/entities/maze.h"
#include "game/online/host.h"
#include "game/online/client.h"

namespace Ui {
class BoardWindow;
}

/**
 * @class BoardWindow
 * @brief Klasa reprezentująca okno gry, dziedzicząca po QDialog.
 *
 * Klasa BoardWindow zarządza głównym oknem gry, w tym inicjalizacją sceny, postaci,
 * obsługą logiki gry, oraz komunikacją sieciową w przypadku gry wieloosobowej.
 * Obsługuje również ruchy postaci, kolizje, oraz aktualizacje stanu gry.
 */
class BoardWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor dla hosta gry.
     * @param name Nazwa gracza.
     * @param port Port, na którym serwer będzie nasłuchiwać połączeń.
     * @param parent Wskaźnik do widgetu nadrzędnego, domyślnie nullptr.
     */
    explicit BoardWindow(QString name, quint16 port, QWidget *parent = nullptr);

    /**
     * @brief Konstruktor dla klienta dołączającego do gry.
     * @param name Nazwa gracza.
     * @param address Adres IP serwera.
     * @param port Port serwera.
     * @param parent Wskaźnik do widgetu nadrzędnego, domyślnie nullptr.
     */
    explicit BoardWindow(QString name, QHostAddress address, quint16 port, QWidget *parent = nullptr);
    ~BoardWindow();

signals:
private slots:
    void loop();
    void restart();
    void weakAllGhosts();

private:
    Ui::BoardWindow *ui;
    QLabel* label;

    QString playerName;

    bool isHost;
    Host host;
    Client client;

    QGraphicsScene *scene;
    Maze maze;
    QList<Ghost*> ghosts;
    QList<Pacman*> pacmans;
    Pacman* playerPacman;
    int playerPacmanId;
    int dotsEaten;

    void initScene();
    void initPacmans();
    void initGhosts();
    void loadCharacters();
    void initTimer();
    void movePacman(Pacman* pacman);
    bool checkPacmanCollisions(Pacman* pacman, Resources::Direction d);
    bool checkGhostCollisions(Ghost *ghost);
    void handleCage();
    Pacman* findPacman(int pacmanId);
    void keyPressEvent(QKeyEvent *event);
    void ghostMovement(Ghost* ghost);
    float calculateDistance(Ghost* ghost, int addX, int addY);
    Ghost* findGhost(Resources::GhostType ghostType);
    void teleportTunnels(Character* character);
    void handleGhostAttack(Ghost* ghost);

    void addNewPacman(QTcpSocket *newclientSocket);
    void addPlayerPacman(int id);

    void backToManu();

    void syncPacmans(const QByteArray &byteArray);
    void syncGhosts(const QByteArray &byteArray);
    void syncDots(const QByteArray &byteArray);

    void setClientPacmanNextDirection(const QByteArray &byteArray);

    void sendPacmans();
    void sendGhosts();
    void sendRemoveDotPosition(int x, int y);

    void reloadScene();

    QTimer timer;
};

#endif // BOARDWINDOW_H
