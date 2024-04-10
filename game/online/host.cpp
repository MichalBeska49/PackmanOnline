#include "game/online/host.h"
#include "game/resources.h"

Host::Host(QObject *parent) : QObject(parent), tcpServer(new QTcpServer(this)) {
    connect(tcpServer, &QTcpServer::newConnection, this, &Host::handleNewConnection);
}

QHostAddress Host::startServer(quint16 port) {
    if (!tcpServer->listen(QHostAddress("127.0.0.1"), 1234)) {
        return QHostAddress::Null;
    }

    return QHostAddress("127.0.0.1");
}

void Host::handleNewConnection() {
    QTcpSocket *newclientSocket = tcpServer->nextPendingConnection();
    connect(newclientSocket, &QTcpSocket::disconnected, newclientSocket, &QTcpSocket::deleteLater);
    connect(newclientSocket, &QTcpSocket::readyRead, this, &Host::readClientData);

    clients.append(newclientSocket);

    emit onNewClient(newclientSocket);
}

void Host::readClientData() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(clientSocket);
    quint8 messageType;
    QByteArray data;

    in >> messageType >> data;

    switch (static_cast<Resources::HostMessageType>(messageType)) {
    case Resources::HostMessageType::PacmanChangeDirection:
        emit onGetPacmanNextDirection(data);
        break;
    }
}

void Host::sendDataToClients(Resources::ClientMessageType type, const QByteArray &data)
{
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);

    stream << static_cast<quint8>(type) << data;

    for (QTcpSocket *client : qAsConst(clients)) {
        client->write(message);
    }
}

void Host::sendPacmanIdToClient(QTcpSocket *newclientSocket, const QByteArray &data){
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);

    stream << static_cast<quint8>(Resources::ClientMessageType::ClientPacmanId) << data;

    for (QTcpSocket *client : qAsConst(clients)) {
        client->write(message);
    }
}
