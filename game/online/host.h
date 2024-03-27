#ifndef HOST_H
#define HOST_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class Host : public QObject
{
    Q_OBJECT
public:
    int id;
    Host(int id, QObject *parent = nullptr);
    bool startServer(quint16 port);

private slots:
    void handleNewConnection();
    void readClientData();

private:
    QTcpServer *tcpServer;
};

#endif // HOST_H
