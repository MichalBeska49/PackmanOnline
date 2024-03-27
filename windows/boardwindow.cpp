#include "boardwindow.h"
#include "ui_boardwindow.h"
#include "game/online/host.h"
#include "game/entities/maze.h"
#include "game/resources.h"

BoardWindow::BoardWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BoardWindow)
    , loopTime(0.0f)
    , dotsEaten(0)
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

    maze.renderMaze(scene);

    ui->gameFrame->setMinimumSize(380, 420);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    ui->gameFrame->setLayout(layout);

    initPacmans();
    initGhosts();
    loadCharacters();

    connect(&timer, &QTimer::timeout, this, &BoardWindow::loop);
    connect(&maze, &Maze::bigDotIsEaten, this, &BoardWindow::weakAllGhosts);

    timer.start(int(1000.0f/Resources::FPS));
}

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::loop() {
    for(Pacman* pacman: pacmans){
        if(!pacman->isDead()){
            movePlayerPacman();
            pacman->setPos(pacman->getScreenPosX(), pacman->getScreenPosY());
        }

        if(maze.removeDot(pacman)){
            dotsEaten++;

            scene->items().clear();
            maze.renderMaze(scene);
            loadCharacters();
        }

        teleportTunnels(pacman);
    }


    for(Ghost* ghost: ghosts){
        if(ghost->isWeak())
        {
            ghost->setDestination(9,7);

            if(ghost->getTileX() == 9 && ghost->getTileY() == 7){
                ghost->setWeak(false);
                ghost->setSpeed(1);
            }

        } else if(!ghost->isScattering()){
            Pacman* pacman = ghost->findClosetPacman(pacmans);

            switch (ghost->getGhostType()) {
            case Resources::GhostType::Blinky:
                ghost->setDestination(pacman->getTileX(), pacman->getTileY());
                break;

            case Resources::GhostType::Pinky:
                switch (pacman->getDirection())
                {
                case Resources::Direction::Up:
                    ghost->setDestination(pacman->getTileX(), pacman->getTileY() - 4);
                    break;
                case Resources::Direction::Down:
                    ghost->setDestination(pacman->getTileX(), pacman->getTileY() + 4);
                    break;
                case Resources::Direction::Left:
                    ghost->setDestination(pacman->getTileX() - 4, pacman->getTileY());
                    break;
                case Resources::Direction::Right:
                    ghost->setDestination(pacman->getTileX() + 4, pacman->getTileY());
                    break;
                }
                break;

            case Resources::GhostType::Inky:
                ghost->setDestination(pacman->getTileX() + (ghost->getTileX() - pacman->getTileX()), pacman->getTileY() + (ghost->getTileY() - pacman->getTileY()));
                break;

            case Resources::GhostType::Clyde:
                if (sqrt(pow((ghost->getTileX() - (pacman->getTileX())), 2) + pow((ghost->getTileY() - (pacman->getTileY())), 2)) < 9)
                {
                    ghost->setDestination(pacman->getTileX(), pacman->getTileY());
                }
                else
                {
                    ghost->setDestination(1, Resources::MAZE_SIZE-1);
                }
                break;
            }
        }

        ghostMovement(ghost);
        ghost->setPos(ghost->getScreenPosX(), ghost->getScreenPosY());

        teleportTunnels(ghost);

        handleGhostAttack(ghost);
    }

    handleCage();
}

void BoardWindow::restart()
{
}

void BoardWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            playerPacman->setNextDirection(Resources::Direction::Up);
            break;
        case Qt::Key_S:
            playerPacman->setNextDirection(Resources::Direction::Down);
            break;
        case Qt::Key_A:
            playerPacman->setNextDirection(Resources::Direction::Left);
            break;
        case Qt::Key_D:
            playerPacman->setNextDirection(Resources::Direction::Right);
            break;
        default:
            QDialog::keyPressEvent(event);
            return;
    }
}

void BoardWindow::movePlayerPacman() {
    if(checkPacmanCollisions(playerPacman->getNextDirection())
        && (int)(playerPacman->getScreenPosX()) % Resources::MAZE_TILE_SIZE == 0
        && (int)(playerPacman->getScreenPosY()) % Resources::MAZE_TILE_SIZE == 0)
    {
        playerPacman->setDirection(playerPacman->getNextDirection());
        playerPacman->move();

    }

    if(!checkPacmanCollisions(playerPacman->getDirection())){
        return;
    }

    playerPacman->move();
}

Pacman* BoardWindow::findPacman(int pacmanId) {
    for (Pacman*& pacman : pacmans) {
        if (pacman->getId() == pacmanId) {
            return pacman;
        }
    }
}

bool BoardWindow::checkPacmanCollisions(Resources::Direction d) {
    if (d != Resources::Direction::Unset)
    {
        switch (d)
        {
        case Resources::Direction::Up:
            return !maze.tileBlocks(playerPacman->getTileX(), playerPacman->getTileY() - 1);
            break;
        case Resources::Direction::Down:
            return !maze.tileBlocks(playerPacman->getTileX(), playerPacman->getTileY() + 1);
            break;
        case Resources::Direction::Left:
            return !maze.tileBlocks(playerPacman->getTileX() - 1, playerPacman->getTileY());
            break;
        case Resources::Direction::Right:
            return !maze.tileBlocks(playerPacman->getTileX() + 1, playerPacman->getTileY());
            break;
        case Resources::Direction::Unset:

            break;
        }

    }
    return true;
}

void BoardWindow::loadCharacters()
{
    for(Pacman* pacman: pacmans){
        pacman->setZValue(1);
        scene->addItem(pacman);
        pacman->setPos(pacman->getScreenPosX(), pacman->getScreenPosY());
    }

    for(Ghost* ghost: ghosts){
        ghost->setZValue(1);
        scene->addItem(ghost);
        ghost->setPos(ghost->getScreenPosX(), ghost->getScreenPosY());
    }
}

void BoardWindow::initPacmans()
{

    //TO DO
    Pacman* yourPacman = new Pacman(0);
    pacmans.append(yourPacman);
    playerPacman = yourPacman;
    playerPacmanId = 0;

}

void BoardWindow::initGhosts()
{
    Ghost* blinky = new Ghost(9, 7, 16, 0, Resources::Direction::Right, Resources::GhostType::Blinky);
    blinky->laveCage();
    ghosts.append(blinky);

    Ghost* clyde = new Ghost(9, 9, 3, 18, Resources::Direction::Left, Resources::GhostType::Clyde);
    ghosts.append(clyde);

    Ghost* inky = new Ghost(10, 9, 16, 18, Resources::Direction::Right, Resources::GhostType::Inky);
    ghosts.append(inky);

    Ghost* pinky = new Ghost(8, 9, 3, 0, Resources::Direction::Left, Resources::GhostType::Pinky);
    ghosts.append(pinky);

    for(Ghost* ghost: ghosts){
        ghost->setZValue(1);
        scene->addItem(ghost);
        ghost->setPos(ghost->getScreenPosX(), ghost->getScreenPosY());
    }
}

void BoardWindow::ghostMovement(Ghost* ghost)
{
    if(!ghost->isOutOfCage()){
        return;
    }

    if (ghost->isScattering())
    {
        if (ghost->getTileX() == ghost->getDestX() && ghost->getTileY() == ghost->getDestY())
        {
            ghost->setScattering(false);
        }
    }

    if (maze.isIntersection(ghost->getTileX(), ghost->getTileY()) || ghost->getChangeMoving())
    {
        if (ghost->shouldTakeDecision())
        {
            float right = calculateDistance(ghost, 1, 0);
            float left = calculateDistance(ghost, -1, 0);
            float up = calculateDistance(ghost, 0, -1);
            float down = calculateDistance(ghost, 0, 1);

            switch (ghost->getMoving())
            {
            case Resources::Direction::Up:
                if (right < left && right < up)
                    ghost->setMoving(Resources::Direction::Right);
                else if (left < right && left < up)
                    ghost->setMoving(Resources::Direction::Left);
                else if (up < left && up < right)
                    ghost->setMoving(Resources::Direction::Up);
                break;

            case Resources::Direction::Down:
                if (right < left && right < down)
                    ghost->setMoving(Resources::Direction::Right);
                else if (left < right && left < down)
                    ghost->setMoving(Resources::Direction::Left);
                else if (down < left && down < right)
                    ghost->setMoving(Resources::Direction::Down);
                break;

            case Resources::Direction::Left:
                if (left < up && left < down)
                    ghost->setMoving(Resources::Direction::Left);
                else if (up < left && up < down)
                    ghost->setMoving(Resources::Direction::Up);
                else if (down < left && down < up)
                    ghost->setMoving(Resources::Direction::Down);
                break;

            case Resources::Direction::Right:
                if (right < up && right < down)
                    ghost->setMoving(Resources::Direction::Right);
                else if (up < right && up < down)
                    ghost->setMoving(Resources::Direction::Up);
                else if (down < up && down < right)
                    ghost->setMoving(Resources::Direction::Down);
                break;

            case Resources::Direction::Unset:

                break;
            }
        }
        ghost->setChangeMoving(false);
        ghost->setTakeDecision(false);
    } else
    {
        ghost->setTakeDecision(true);
    }


    if (checkGhostCollisions(ghost))
        ghost->move();
    else{
        ghost->setChangeMoving(true);
        ghost->setTakeDecision(true);
    }

}

float BoardWindow::calculateDistance(Ghost *ghost, int addX, int addY)
{
    float distance = std::numeric_limits<float>::max();;
    if (!maze.tileBlocks(ghost->getTileX() + addX, ghost->getTileY() + addY))
    {
        distance = (float) sqrt(pow((ghost->getDestX() - (ghost->getTileX() + addX)), 2) + pow((ghost->getDestY() - (ghost->getTileY() + addY)), 2));
    }
    return distance;
}

bool BoardWindow::checkGhostCollisions(Ghost *ghost)
{
    switch (ghost->getMoving())
    {
    case Resources::Direction::Up:
        return !maze.tileBlocks(ghost->getTileX(), ghost->getTileY() - 1);
        break;
    case Resources::Direction::Down:
        return !maze.tileBlocks(ghost->getTileX(), ghost->getTileY() + 1);
        break;
    case Resources::Direction::Left:
        return !maze.tileBlocks(ghost->getTileX() - 1, ghost->getTileY());
        break;
    case Resources::Direction::Right:
        return !maze.tileBlocks(ghost->getTileX() + 1, ghost->getTileY());
        break;
    default:
        return false;
    }
}

void BoardWindow::handleCage() {
    if(dotsEaten == 10)
    {
        Ghost* inky = findGhost(Resources::GhostType::Inky);

        if(!inky->isOutOfCage()){
           inky->laveCage();
           inky->teleport(9, 7);
        }
    } else if(dotsEaten == 25)
    {
        Ghost* pinky = findGhost(Resources::GhostType::Pinky);

        if(!pinky->isOutOfCage()){
            pinky->laveCage();
            pinky->teleport(9, 7);
        }
    } else if(dotsEaten == 75)
    {
        Ghost* clyde = findGhost(Resources::GhostType::Clyde);

        if(!clyde->isOutOfCage()){
            clyde->laveCage();
            clyde->teleport(9, 7);
        }
    }
}

Ghost* BoardWindow::findGhost(Resources::GhostType ghostType){
    for (Ghost*& ghost : ghosts) {
        if (ghost->getGhostType() == ghostType) {
            return ghost;
        }
    }
}

void BoardWindow::teleportTunnels(Character* character)
{
    if (character->getTileX() == 0 && character->getTileY() == 9)
        character->teleport(17, 9);
    else if (character->getTileX() == 18 && character->getTileY() == 9)
        character->teleport(1, 9);

    character->setPos(character->getScreenPosX(), character->getScreenPosY());
}


void BoardWindow::handleGhostAttack(Ghost* ghost)
{
    for (Pacman*& pacman : pacmans) {
        if (pacman->getTileX() == ghost->getTileX() && pacman->getTileY() == ghost->getTileY())
        {
            if (ghost->isWeak())
            {
                ghost->teleport(9, 7);
                ghost->setWeak(false);
                ghost->setSpeed(1);
                pacman->setScore(pacman->getScore() + 100);
            }
            else if(!pacman->isDead())
            {
                pacman->setDead(true);
                pacman->setVisible(false);
            }
        }
    }
}

void BoardWindow::weakAllGhosts()
{
    for (Ghost*& ghost : ghosts) {
        if(ghost->isOutOfCage()){
            ghost->setWeak(true);
            ghost->setSpeed(0.4);
            ghost->setChangeMoving(true);
        }
    }
}
