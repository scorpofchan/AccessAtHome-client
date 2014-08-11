#ifndef CLIENT_H
#define CLIENT_H

#include "global.h"
#include "rsabox.h"
#include "aesbox.h"
#include <QDebug>
#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QNetworkAccessManager>

#define SERVER_HOST "rchlie7hgfhecoi7.onion"
#define SERVER_PORT 4444

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(int , QString, QObject *parent = 0);
    int handshake();
    int doAction();

signals:
    void finished(int);
    void finished();

public slots:
    void start();
    void finish();
    void finish(int);

private:
    int cmd;
    QString payload;
    _command *command;
    QTcpSocket *socket;
    RSABox rsa;
    AESBox aes;
};

#endif // CLIENT_H
