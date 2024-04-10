#include "boardwindow.h"
#include "ui_boardwindow.h"
#include "game/entities/maze.h"
#include "game/resources.h"
#include "mainwindow.h"

BoardWindow::BoardWindow(QString name, quint16 port, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BoardWindow)
    , dotsEaten(0)
    , isHost(true)
    , playerName(name)
{
    ui->setupUi(this);

    ui->label->setText(host.startServer(port).toString());

    connect(&host, &Host::onNewClient, this, &BoardWindow::addNewPacman);
    connect(&host, &Host::onGetPacmanNextDirection, this, &BoardWindow::setClientPacmanNextDirection);

    if (ui->label->text() != "") { // Uruchomienie serwera na porcie 12345
        qDebug() << "Server started on port";
    }

    Pacman* yourPacman = new Pacman(pacmans.count(), name);
    pacmans.append(yourPacman);
    playerPacman = yourPacman;
    playerPacmanId = yourPacman->getId();

    initScene();

    initPacmans();
    initGhosts();
    loadCharacters();

    initTimer();
}

BoardWindow::BoardWindow(QString name, QHostAddress address, quint16 port, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BoardWindow)
    , dotsEaten(0)
    , isHost(false)
    , playerName(name)
{
    ui->setupUi(this);

    client.connectToServer(address, port);

    connect(&client, &Client::onGetClientPacmanId, this, &BoardWindow::addPlayerPacman);
    connect(&client, &Client::onGetPacmansFromHost, this, &BoardWindow::syncPacmans);
    connect(&client, &Client::onGetGhostsFromHost, this, &BoardWindow::syncGhosts);
    connect(&client, &Client::onGetRemoveDotFromHost, this, &BoardWindow::syncDots);

    initScene();
    initGhosts();

    initTimer();
}

BoardWindow::~BoardWindow()
{
    delete ui;
}

void BoardWindow::backToManu(){
    MainWindow* mainWindow = new MainWindow(this);
    mainWindow->show();

    QWidget* boardWindow = ui->gameFrame->parentWidget();
    boardWindow->close();
}

void BoardWindow::initTimer(){
    connect(&timer, &QTimer::timeout, this, &BoardWindow::loop);
    connect(&maze, &Maze::bigDotIsEaten, this, &BoardWindow::weakAllGhosts);

    timer.start(int(1000.0f/Resources::FPS));
}

void BoardWindow::initScene(){

    QGraphicsView *view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    maze.renderMaze(scene);

    ui->gameFrame->setMinimumSize(380, 420);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    ui->gameFrame->setLayout(layout);
}

void BoardWindow::loop() {
    if(isHost){
        for(Pacman* pacman: pacmans){
            if(!pacman->isDead()){
                movePacman(pacman);
                pacman->setPos(pacman->getScreenPosX(), pacman->getScreenPosY());
            }

            if(maze.removeDot(pacman)){
                dotsEaten++;

                reloadScene();

                sendRemoveDotPosition(pacman->getTileX(), pacman->getTileY());
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

        sendPacmans();
        sendGhosts();

    } else {
        for(Pacman* pacman: pacmans){
            pacman->setPos(pacman->getScreenPosX(), pacman->getScreenPosY());
        }

        for(Ghost* ghost: ghosts){
            ghost->setPos(ghost->getScreenPosX(), ghost->getScreenPosY());
        }
    }
}

void BoardWindow::restart()
{
}

void BoardWindow::keyPressEvent(QKeyEvent *event) {
    Resources::Direction playerNextDirection;

    switch (event->key()) {
        case Qt::Key_W:
            playerNextDirection = Resources::Direction::Up;
            break;
        case Qt::Key_S:
            playerNextDirection = Resources::Direction::Down;
            break;
        case Qt::Key_A:
            playerNextDirection = Resources::Direction::Left;
            break;
        case Qt::Key_D:
            playerNextDirection = Resources::Direction::Right;
            break;
        default:
            QDialog::keyPressEvent(event);
            return;
    }
    if(isHost){
        playerPacman->setNextDirection(playerNextDirection);
    } else {
        QJsonObject json;

        json["pacmanId"] = playerPacmanId;
        json["nextDirection"] = int(playerNextDirection);

        QJsonDocument jsonDoc(json);
        QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

        client.sendDataToHost(Resources::HostMessageType::PacmanChangeDirection, jsonString.toUtf8());
    }
}

void BoardWindow::movePacman(Pacman* pacman) {
    if(checkPacmanCollisions(pacman, pacman->getNextDirection())
        && (int)(pacman->getScreenPosX()) % Resources::MAZE_TILE_SIZE == 0
        && (int)(pacman->getScreenPosY()) % Resources::MAZE_TILE_SIZE == 0)
    {
        pacman->setDirection(pacman->getNextDirection());
        pacman->move();
        return;
    }

    if(!checkPacmanCollisions(pacman, pacman->getDirection())){
        return;
    }

    pacman->move();
}

Pacman* BoardWindow::findPacman(int pacmanId) {
    for (Pacman*& pacman : pacmans) {
        if (pacman->getId() == pacmanId) {
            return pacman;
        }
    }
}

bool BoardWindow::checkPacmanCollisions(Pacman* pacman, Resources::Direction d) {
    if (d != Resources::Direction::Unset)
    {
        switch (d)
        {
        case Resources::Direction::Up:
            return !maze.tileBlocks(pacman->getTileX(), pacman->getTileY() - 1);
            break;
        case Resources::Direction::Down:
            return !maze.tileBlocks(pacman->getTileX(), pacman->getTileY() + 1);
            break;
        case Resources::Direction::Left:
            return !maze.tileBlocks(pacman->getTileX() - 1, pacman->getTileY());
            break;
        case Resources::Direction::Right:
            return !maze.tileBlocks(pacman->getTileX() + 1, pacman->getTileY());
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
    for(Pacman* pacman: pacmans){
        pacman->setZValue(1);
        scene->addItem(pacman);
        pacman->setPos(pacman->getScreenPosX(), pacman->getScreenPosY());
    }
}

void BoardWindow::initGhosts()
{
    Ghost* blinky = new Ghost(9, 7, 16, 1, Resources::Direction::Right, Resources::GhostType::Blinky);
    blinky->laveCage();
    ghosts.append(blinky);

    Ghost* clyde = new Ghost(9, 9, 3, 17, Resources::Direction::Left, Resources::GhostType::Clyde);
    ghosts.append(clyde);

    Ghost* inky = new Ghost(10, 9, 16, 17, Resources::Direction::Right, Resources::GhostType::Inky);
    ghosts.append(inky);

    Ghost* pinky = new Ghost(8, 9, 3, 1, Resources::Direction::Left, Resources::GhostType::Pinky);
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

            switch (ghost->getDirection())
            {
            case Resources::Direction::Up:
                if (right < left && right < up)
                    ghost->setDirection(Resources::Direction::Right);
                else if (left < right && left < up)
                    ghost->setDirection(Resources::Direction::Left);
                else if (up < left && up < right)
                    ghost->setDirection(Resources::Direction::Up);
                break;

            case Resources::Direction::Down:
                if (right < left && right < down)
                    ghost->setDirection(Resources::Direction::Right);
                else if (left < right && left < down)
                    ghost->setDirection(Resources::Direction::Left);
                else if (down < left && down < right)
                    ghost->setDirection(Resources::Direction::Down);
                break;

            case Resources::Direction::Left:
                if (left < up && left < down)
                    ghost->setDirection(Resources::Direction::Left);
                else if (up < left && up < down)
                    ghost->setDirection(Resources::Direction::Up);
                else if (down < left && down < up)
                    ghost->setDirection(Resources::Direction::Down);
                break;

            case Resources::Direction::Right:
                if (right < up && right < down)
                    ghost->setDirection(Resources::Direction::Right);
                else if (up < right && up < down)
                    ghost->setDirection(Resources::Direction::Up);
                else if (down < up && down < right)
                    ghost->setDirection(Resources::Direction::Down);
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
    switch (ghost->getDirection())
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

void BoardWindow::addNewPacman(QTcpSocket *newClientSocket){
    Pacman* newPacman = new Pacman(pacmans.count(), "gosc");
    pacmans.append(newPacman);

    host.sendPacmanIdToClient(newClientSocket, QByteArray::number(newPacman->getId()));
}

void BoardWindow::addPlayerPacman(int id){
    Pacman* yourPacman = new Pacman(id, playerName);
    playerPacman = yourPacman;
    playerPacmanId = yourPacman->getId();
}

void BoardWindow::syncPacmans(const QByteArray &byteArray){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonArray jsonArray = jsonDoc.array();

    QList<Pacman*> pacmanList;
    for (const QJsonValue &value : jsonArray) {
        Pacman* newPacman = new Pacman();
        newPacman->fromJsonObject(value.toObject());
        pacmanList.append(newPacman);

        for (Pacman*& pacman : pacmans) {
            if(newPacman->getId() == pacman->getId()){
                pacman->reaload(newPacman);
            }
        }
    }

    if(pacmans.count() != 0){
        return;
    }

    pacmans = pacmanList;
    initPacmans();
}

void BoardWindow::syncGhosts(const QByteArray &byteArray){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue &value : jsonArray) {
        Ghost* ghostToUpdate = new Ghost();
        ghostToUpdate->fromJsonObject(value.toObject());

        for (Ghost*& ghost : ghosts) {
            if(ghostToUpdate->getGhostType() == ghost->getGhostType()){
                ghost->reaload(ghostToUpdate);
            }
        }
    }
}

void BoardWindow::syncDots(const QByteArray &byteArray){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    const QJsonValue &value = jsonDoc.array();
    QJsonObject json = jsonDoc.object();

    maze.removeDot(json["x"].toInt(), json["y"].toInt());

    reloadScene();
}

void BoardWindow::sendPacmans(){
    QJsonArray jsonArray;
    for (Pacman* &pacman : pacmans) {
        jsonArray.append(pacman->toJsonObject());
    }

    QJsonDocument jsonDoc(jsonArray);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    host.sendDataToClients(Resources::ClientMessageType::Pacmans, jsonString.toUtf8());
}

void BoardWindow::sendGhosts(){
    QJsonArray jsonArray;
    for (Ghost* &ghost : ghosts) {
        jsonArray.append(ghost->toJsonObject());
    }

    QJsonDocument jsonDoc(jsonArray);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    host.sendDataToClients(Resources::ClientMessageType::Ghosts, jsonString.toUtf8());
}

void BoardWindow::sendRemoveDotPosition(int x, int y){
    QJsonObject json;

    json["x"] = x;
    json["y"] = y;

    QJsonDocument jsonDoc(json);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    host.sendDataToClients(Resources::ClientMessageType::PacmanEatDot, jsonString.toUtf8());
}

void BoardWindow::setClientPacmanNextDirection(const QByteArray &byteArray){
    QString jsonString = QString::fromUtf8(byteArray);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    QJsonArray jsonArray = jsonDoc.array();
    QJsonObject json = jsonDoc.object();
    int pacmanId = json["pacmanId"].toInt();
    Resources::Direction nextDirection = static_cast<Resources::Direction>(json["nextDirection"].toInt());

    for (Pacman* &pacman : pacmans) {
        if(pacman->getId() == pacmanId){
            pacman->setNextDirection(nextDirection) ;
        }
    }
}

void BoardWindow::reloadScene(){
    scene->items().clear();
    maze.renderMaze(scene);
    loadCharacters();
}

