#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    aes.aesInit();
    int x = 0;
    x = rsa.setPrivKey();
    x = rsa.setPubKey();
}

void Client::start(){
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    socket->connectToHost("192.168.158.1", 4444);
    if(!socket->waitForConnected(10000))
       {
           qDebug() << "Error: " << socket->errorString();
       }
}

void Client::connected() {
    qDebug() << "connected...";
    int len = 0;
    unsigned char *data = NULL;
    data = (unsigned char*)malloc(AES_KEYLEN/8 + AES_BLOCK_SIZE);
    if (data == NULL) goto con_err;
    memcpy(data, aes.getkey(), AES_KEYLEN/8);
    memcpy(data + AES_KEYLEN/8, aes.getiv(), AES_BLOCK_SIZE);
    rsa.setdata((char *)data, AES_KEYLEN/8 + AES_BLOCK_SIZE);
    len = rsa.rsaEncrypt();
    if (len == 0) goto con_err;
    socket->write((char *)rsa.getenc(), len);
    rsa.freedata();
    rsa.freeenc();
    free(data);
    data = NULL;
con_err:
    rsa.freedata();
    rsa.freeenc();
    if (data != NULL) free(data);
    data = NULL;
}

void Client::disconnected() {
    qDebug() << "disconnected...";
}

void Client::bytesWritten(qint64 bytes) {
    //qDebug() << bytes << " bytes written...";
}

void Client::readyRead() {
    qDebug() << "reading...";
    int len = 0;
    unsigned char *data;
    data = NULL;
    data = (unsigned char*)malloc(1025);
    len = socket->read((char *)data, 1024);
    data[len] = 0;
    aes.setenc((char *)data,len);
    len = aes.aesDecrypt();
    if (strncmp((char *)aes.getdec(),"OK",2)==0) qDebug() << "ok";
}

void Client::doRegister(){

}

void Client::doLogin(QString email, QString pass){
     QString data = "LOGIN\n";
     int len = 0;
     data.append(email);
     data.append("\n");
     data.append(pass);
     aes.setenc((char *)data.toStdString().c_str(),data.size());
     len = aes.aesEncrypt();
     socket->write((char *)aes.getenc(), len);
}
