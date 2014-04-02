#ifndef CLIENT_H
#define CLIENT_H

#include "global.h"
#include "rsabox.h"
#include "aesbox.h"
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include <QString>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    void start();
    void doRegister();
    void doLogin(QString, QString);
signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    RSABox rsa;
    AESBox aes;
};

#endif // CLIENT_H
