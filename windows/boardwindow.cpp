#include "boardwindow.h"
#include "ui_boardwindow.h"
#include "game/online/host.h"
#include "game/entities/maze.h"
#include "game/resources.h"

BoardWindow::BoardWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BoardWindow)
    , maze(new Maze(this))
    , loopTime(0.0f)
{
    ui->setupUi(this);

    Host server(0);
    if (!server.startServer(12345)) { // Uruchomienie serwera na porcie 12345
        qDebug() << "Server started on port";
    }

    QGraphicsView *view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    maze->createMaze(scene);

    ui->gameFrame->setMinimumSize(380, 420);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    ui->gameFrame->setLayout(layout);

    Pacman* yourPacman = new Pacman(0);
    pacmans.append(yourPacman);
    playerPacmanId = 0;

    //yourPacman->setPos(yourPacman->position);
    //scene->addItem(yourPacman);

    //connect(&timer, &QTimer::timeout, this, &BoardWindow::loop);
    //timer.start(int(1000.0f/Resources::FPS));
}

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::loop() {

}

void BoardWindow::keyPressEvent(QKeyEvent *event) {
    QPoint newPacmanPosition = QPoint(1, 1);// findPacman(playerPacmanId)->position; // Kopiujemy obecną pozycję, aby sprawdzić potencjalny nowy ruch

    switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            newPacmanPosition -= QPoint(0, 1); // Ruch w górę zmniejsza wartość y
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            newPacmanPosition += QPoint(0, 1); // Ruch w dół zwiększa wartość y
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            newPacmanPosition -= QPoint(1, 0); // Ruch w lewo zmniejsza wartość x
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            newPacmanPosition += QPoint(1, 0); // Ruch w prawo zwiększa wartość x
            break;
        default:
            QDialog::keyPressEvent(event); // Dla innych klawiszy, wywołaj domyślną obsługę
            return;
    }

    if (checkCollisions(newPacmanPosition)) {
        movePacman(newPacmanPosition);
    }
}

void BoardWindow::movePacman(QPoint newPosition) {
    Pacman* pacman = findPacman(playerPacmanId);

    //pacman->position = newPosition;
    pacman->setPos(newPosition);
}

void BoardWindow::moveGhosts() {

}

Pacman* BoardWindow::findPacman(int pacmanId) {
    for (Pacman*& pacman : pacmans) {
        if (pacman->id == pacmanId) {
            return pacman;
        }
    }
}

bool BoardWindow::checkCollisions(QPoint newPosition) {
    int x = ((newPosition.x() + maze->TITLE_SIZE - 1) /  maze->TITLE_SIZE) - 1;
    int y = ((newPosition.y() + maze->TITLE_SIZE - 1) /  maze->TITLE_SIZE) - 1;

    if (x >= 0 && x < maze->MAZE_SIZE &&
        y >= 0 && y < maze->MAZE_SIZE) {
        return maze->map[y][x] != 1;
    }

    return false;
}
