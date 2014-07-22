#ifndef CLIENT_H
#define CLIENT_H

#include "global.h"
#include "database.h"
#include "rsabox.h"
#include "aesbox.h"
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkProxy>

//#define SERVER_HOST "192.168.158.1"
#define SERVER_HOST "rchlie7hgfhecoi7.onion"
#define SERVER_PORT 4444

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    explicit Client(int , QString, QObject *parent = 0);
    int handshake();
    int doAction();

signals:
    void finished(int);
    void finished();

public slots:
    void start();
    void genkey();
    void finish(int);
    void finish();
    void hello();

private:
    QTcpSocket *socket;
    _command *command;
    RSABox rsa;
    AESBox aes;
};

#endif // CLIENT_H
