#include "listener.h"

Listener::Listener(QObject *parent) :
    QTcpServer(parent) {
}

void Listener::start() {
    int port = 4444;
    if(!this->listen(QHostAddress::LocalHost, port)) {
        qDebug() << "[*] Could not start listen";
    }
}

void Listener::incomingConnection(qintptr socketDescriptor) {
    qDebug() << socketDescriptor << "[*] Connecting...";
    handler = new Handler(socketDescriptor, this);
    //connect(handler, SIGNAL(finish()), handler, SLOT(terminate()), Qt::DirectConnection);
    //connect(handler, SIGNAL(finish()), handler, SLOT(deleteLater()));
    handler->start();
}
