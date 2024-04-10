#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDebug>
#include "game/resources.h"

/**
 * @class Client
 * @brief Klasa reprezentująca klienta w architekturze klient-serwer gry.
 *
 * Klasa Client zarządza połączeniem sieciowym pomiędzy klientem a serwerem, umożliwiając
 * wysyłanie i odbieranie danych dotyczących stanu gry. Zajmuje się również obsługą błędów połączenia.
 */
class Client : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor klasy Client.
     * @param parent Wskaźnik do obiektu nadrzędnego, domyślnie nullptr.
     */
    Client(QObject *parent = nullptr);

    void connectToServer(const QHostAddress &serverAddress, quint16 serverPort);
    void sendDataToHost(Resources::HostMessageType type, const QByteArray &data);

signals:
    void onErrorOccurred();
    void onGetClientPacmanId(int id);
    void onGetPacmansFromHost(const QByteArray &data);
    void onGetGhostsFromHost(const QByteArray &data);
    void onGetRemoveDotFromHost(const QByteArray &data);

private slots:
    void onConnected();
    void readData();
    void onSocketError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket tcpClient;
};

#endif // CLIENT_H
