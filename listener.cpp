#include "listener.h"

Listener::Listener(QObject *parent) :
    QTcpServer(parent)
{
}

void Listener::start()
{
    int port = 4444;
    if(!this->listen(QHostAddress::LocalHost,port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";

    }
}

void Listener::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    Thread *thread = new Thread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
