#ifndef HOST_H
#define HOST_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include "game/resources.h"

/**
 * @class Host
 * @brief Klasa reprezentująca serwer w architekturze klient-serwer gry.
 *
 * Klasa Host zarządza serwerem gry, umożliwiając nawiązywanie połączeń z klientami,
 * wysyłanie danych do wszystkich połączonych klientów oraz obsługę nowych połączeń.
 * Służy jako centralny punkt dystrybucji danych gry między wszystkimi klientami.
 */
class Host : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor klasy Host.
     * @param parent Wskaźnik do obiektu nadrzędnego, domyślnie nullptr.
     */
    Host(QObject *parent = nullptr);

    QHostAddress startServer(quint16 port);

    void sendDataToClients(Resources::ClientMessageType type, const QByteArray &data);
    void sendPacmanIdToClient(QTcpSocket *newclientSocket, const QByteArray &data);

signals:
    void onNewClient(QTcpSocket *newclientSocket);
    void onGetPacmanNextDirection(const QByteArray &data);

private slots:
    void handleNewConnection();
    void readClientData();

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;
};

#endif // HOST_H
