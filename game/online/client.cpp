#include "game/online/client.h"

Client::Client(QObject *parent)
    : QObject(parent)
{
    connect(&tcpClient, &QTcpSocket::connected, this, &Client::onConnected);
    connect(&tcpClient, &QTcpSocket::readyRead, this, &Client::readData);
    connect(&tcpClient, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &Client::onSocketError);
}

void Client::connectToServer(const QHostAddress &serverAddress, quint16 serverPort){
    tcpClient.connectToHost(QHostAddress("127.0.0.1"), 1234);
}

void Client::sendDataToHost(Resources::HostMessageType type, const QByteArray &data)
{
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);

    stream << static_cast<quint8>(type) << data;

    tcpClient.write(message);
}

void Client::onConnected() {

}

void Client::readData()
{
    if (tcpClient.bytesAvailable() == 0) {
        return;
    }

    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(clientSocket);
    quint8 messageType;
    QByteArray data;

    in >> messageType >> data;

    switch (static_cast<Resources::ClientMessageType>(messageType)) {
        case Resources::ClientMessageType::Pacmans:
            emit onGetPacmansFromHost(data);
            break;
        case Resources::ClientMessageType::Ghosts:
            emit onGetGhostsFromHost(data);
            break;
        case Resources::ClientMessageType::ClientPacmanId:
            int id;
            in >> id;

            emit onGetClientPacmanId(id);
            break;
        case Resources::ClientMessageType::PacmanEatDot:
            emit onGetRemoveDotFromHost(data);
            break;
    }
}

void Client::onSocketError(QAbstractSocket::SocketError socketError){
    qDebug() << "Wystąpił błąd połączenia:" << socketError;

    emit onErrorOccurred();
}
