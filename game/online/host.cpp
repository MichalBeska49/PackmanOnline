#include "game/online/host.h"

Host::Host(int id, QObject *parent) : QObject(parent), id(id), tcpServer(new QTcpServer(this)) {
    connect(tcpServer, &QTcpServer::newConnection, this, &Host::handleNewConnection);
}

bool Host::startServer(quint16 port) {
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qWarning() << "Server could not start on port" << port << ":" << tcpServer->errorString();
        return false;
    }
    qDebug() << "Server started on port" << port;
    return true;
}

void Host::handleNewConnection() {
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Host::readClientData);
    qDebug() << "New connection from" << clientSocket->peerAddress().toString();
}

void Host::readClientData() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    QDataStream in(clientSocket);
    // Przykład odczytu danych
    // Zakładamy, że wiemy, jakiego typu dane otrzymujemy; w praktyce może być potrzebna dodatkowa logika do obsługi różnych formatów/komend
    QString message;
    in >> message;
    qDebug() << "Received message:" << message;
}
