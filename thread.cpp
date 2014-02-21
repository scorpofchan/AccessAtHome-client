#include "thread.h"


Thread::Thread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    myfile.open("C:/Users/root/QtProjects/Core/a.txt");
    this->socketDescriptor = ID;
}

void Thread::run()
{
    qDebug() << " Thread started";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    qDebug() << socketDescriptor << " Client connected";
    exec();
}

void Thread::readyRead()
{
    QByteArray Data = socket->readAll();
    qDebug() << socketDescriptor << " Data in: " << Data;
    if (myfile.is_open())
    {
      myfile << Data.data();
    }

    socket->write(Data);
}

void Thread::disconnected()
{


    qDebug() << " OFF\n ";
    myfile.close();

    qDebug() << socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}

