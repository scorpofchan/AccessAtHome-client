#ifndef LISTENER_H
#define LISTENER_H

#include <QtNetwork/QTcpServer>
#include "thread.h"

class Listener : public QTcpServer
{
    Q_OBJECT
public:
    explicit Listener(QObject *parent = 0);
    void start();
signals:

public slots:

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // LISTENER_H
