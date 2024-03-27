#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QVector>
#include <QTimer>
#include "game/entities/pacman.h"
#include "game/entities/ghost.h"
#include "game/entities/maze.h"

namespace Ui {
class BoardWindow;
}

class BoardWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BoardWindow(QWidget *parent = nullptr);
    ~BoardWindow();

signals:
private slots:
    void loop();
    void restart();
    void weakAllGhosts();

private:
    Ui::BoardWindow *ui;
    QLabel* label;

    QGraphicsScene *scene;
    Maze maze;
    QList<Ghost*> ghosts;
    QList<Pacman*> pacmans;
    Pacman* playerPacman;
    int playerPacmanId;
    int dotsEaten;

    void initPacmans();
    void initGhosts();
    void loadCharacters();
    void movePlayerPacman();
    bool checkPacmanCollisions(Resources::Direction direction);
    bool checkGhostCollisions(Ghost *ghost);
    void handleCage();
    Pacman* findPacman(int pacmanId);
    void keyPressEvent(QKeyEvent *event);
    void ghostMovement(Ghost* ghost);
    float calculateDistance(Ghost* ghost, int addX, int addY);
    Ghost* findGhost(Resources::GhostType ghostType);
    void teleportTunnels(Character* character);
    void handleGhostAttack(Ghost* ghost);

    QTimer timer;
    float loopTime;
};

#endif // BOARDWINDOW_H
