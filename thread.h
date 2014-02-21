#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <iostream>
#include <fstream>

using namespace std;

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(qintptr ID, QObject *parent = 0);

    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    ofstream myfile;
};

#endif // THREAD_H
