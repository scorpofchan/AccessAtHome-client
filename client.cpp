#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    nonce = NULL;
    aes.aesInit();
    int x = 0;
    x = rsa.setPrivKey();
    x = rsa.setPubKey();
}

int Client::start(int action, QString data){
    //connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName("127.0.0.1");
    proxy.setPort(9250);
    QNetworkProxy::setApplicationProxy(proxy);
    socket = new QTcpSocket(this);
    //socket->connectToHost("192.168.158.1", 4444);
    socket->connectToHost("aogvikhz7xr4usau.onion", 4444);
    if (socket->waitForConnected(10000)) {
        int ret;
        ret = handshake();
        if (ret == 1) {
            switch (action) {
              case REGISTERcmd:
                qDebug() << "[*] Registering...";
                return doRegister(data);
                break;
              case LOGINcmd:
                qDebug() << "[*] Logging...";
                return doLogin(data);
              default:
                break;
            }

        }
        socket->close();
        return 1;
    }
    else qDebug() << "Error: " << socket->errorString();
    return 0;
}

int Client::handshake() {
    qDebug() << "[*] Connected";
    int n = 0;
    unsigned char *data = NULL;
    data = (unsigned char*)malloc(AES_KEYLEN/8 + AES_BLOCK_SIZE);
    if (data == NULL) goto con_err;
    memcpy(data, aes.getkey(), AES_KEYLEN/8);
    memcpy(data + AES_KEYLEN/8, aes.getiv(), AES_BLOCK_SIZE);
    rsa.setdata((char *)data, AES_KEYLEN/8 + AES_BLOCK_SIZE);
    n = rsa.rsaEncrypt();
    if (n == 0) goto con_err;
    if (socket->write((char *)rsa.getenc(), n) != n) goto con_err;
    rsa.freedata();
    rsa.freeenc();
    free(data);
    data = NULL;
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(3000);
    qDebug() << "[*] Reading...";
    data = (unsigned char*)malloc(512);
    n = socket->read((char *)data, 512);
    if (n == 0) goto con_err;
    qDebug() << "[*] " << n;
    aes.setenc((char *)data, n);
    n = aes.aesDecrypt();
    if (n == 0) goto con_err;
    if (strncmp((char *)aes.getdec(), OK, sizeof(OK) - 1)==0) {
        nonce = (unsigned char*)malloc(16);
        if (nonce == NULL) {socket->close(); return 0;};
        memcpy(nonce, aes.getdec() + 2, 16);
        return 1;
    }
con_err:
    rsa.freedata();
    rsa.freeenc();
    if (data != NULL) free(data);
    data = NULL;
    return 0;
}

int Client::doRegister(QString data){
     unsigned char *datax;
     datax = NULL;
     datax = (unsigned char*)malloc(26 + data.size());
     int len = 0;
     memcpy(datax, "REGISTER\n", 9);
     memcpy(datax + 9, nonce, 16);
     datax[25] = '\n';
     memcpy(datax + 26, data.toStdString().c_str(), data.size());
     aes.setdata((char *)datax, 26 + data.size());
     len = aes.aesEncrypt();
     socket->write((char *)aes.getenc(), len);
     socket->waitForBytesWritten(1000);
     socket->waitForReadyRead(3000);
     qDebug() << "[*] Reading again...";
     free(datax);
     datax = NULL;
     datax = (unsigned char*)malloc(1024);
     len = socket->read((char *)datax, 1024);
     aes.setenc((char *)datax,len);
     len = aes.aesDecrypt();
     free(datax);
     if (isEqual(aes.getdec(), (unsigned char *)REGISTERSUCCEED, sizeof(REGISTERSUCCEED) - 1)) {
        qDebug() << "[*] OK LOGIN";
        return REGISTER_SUCCEED;
     }
     else if (isEqual(aes.getdec(), (unsigned char *)REGISTERFAILED, sizeof(REGISTERFAILED) - 1)) {
        qDebug() << "[*] BAD LOGIN";
        return REGISTER_FAILED;
     }
     else if (isEqual(aes.getdec(), (unsigned char*)NONCE, sizeof(NONCE) - 1)) {
        qDebug() << "[*] BAD NONCE";
        return 0;
     }
     return 0;
}

int Client::doLogin(QString data) {
     unsigned char *datax;
     datax = NULL;
     datax = (unsigned char*)malloc(23 + data.size());
     int len = 0;
     memcpy(datax, "LOGIN\n", 6);
     memcpy(datax + 6, nonce, 16);
     datax[22] = '\n';
     memcpy(datax + 23, data.toStdString().c_str(), data.size());
     aes.setdata((char *)datax, 23 + data.size());
     len = aes.aesEncrypt();
     socket->write((char *)aes.getenc(), len);
     socket->waitForBytesWritten(1000);
     socket->waitForReadyRead(3000);
     qDebug() << "[*] Reading again...";
     free(datax);
     datax = NULL;
     datax = (unsigned char*)malloc(1024);
     len = socket->read((char *)datax, 1024);
     aes.setenc((char *)datax,len);
     len = aes.aesDecrypt();
     free(datax);
     if (isEqual(aes.getdec(), (unsigned char *)LOGINSUCCEED, sizeof(LOGINSUCCEED) - 1)) {
         qDebug() << "[*] OK LOGIN";
         return LOGIN_SUCCEED;
     }
     else if (isEqual(aes.getdec(), (unsigned char *)LOGINFAILED, sizeof(LOGINFAILED) - 1)) {
         qDebug() << "[*] BAD LOGIN";
         return LOGIN_FAILED;
     }
     else if (isEqual(aes.getdec(), (unsigned char*)NONCE, sizeof(NONCE) - 1)) {
         qDebug() << "[*] BAD NONCE";
         return 0;
     }
     return 0;
}

void Client::disconnected()
{
    qDebug() << "disconnected...";
}
