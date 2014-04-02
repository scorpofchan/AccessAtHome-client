#ifndef CLIENT_H
#define CLIENT_H

#include "global.h"
#include "rsabox.h"
#include "aesbox.h"
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkProxy>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    int start(int, QString);
    int doRegister(QString data);
    int doLogin(QString data);
    int handshake();
signals:

public slots:
    void disconnected();

private:
    QTcpSocket *socket;
    unsigned char *nonce;
    RSABox rsa;
    AESBox aes;
};

#endif // CLIENT_H
