#ifndef HANDLER_H
#define HANDLER_H

#include "global.h"
#include "rsabox.h"
#include "aesbox.h"
#include "http.h"
#include "job.h"
#include <QThread>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QDir>

using namespace std;

class Handler : public QThread
{
    Q_OBJECT
public:
    explicit Handler(qintptr , QObject *parent = 0);
    void run();
    int handshake();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void finish();
    void close();

public slots:
    void requestHandler();
    void disconnected();
    void handler(unsigned char *, int);
    void newjobHandler();
    void hello();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    RSABox rsa;
    AESBox aes;
    _command *command;

};

#endif // HANDLER_H
