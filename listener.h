#ifndef LISTENER_H
#define LISTENER_H

#include "handler.h"

class Listener : public QTcpServer
{
    Q_OBJECT
public:
    explicit Listener(QObject *parent = 0);
signals:

public slots:
    void start();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    Handler *handler;

};

#endif // LISTENER_H
