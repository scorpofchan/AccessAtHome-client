#include "client.h"

Client::Client(int _cmd, QString _payload, QObject *parent) : QObject(parent) {
    cmd = _cmd;
    payload = _payload;
}

void Client::start() {
    aes.aesInit();
    rsa.setPrivKey(PRIVATEKEY);
    rsa.setPubKey(PUBLICKEY);
    command = NULL;
    command = (_command *)malloc(sizeof(_command));
    if (command == NULL) return;
    command->code[0] = (unsigned char)cmd;
    command->success[0] = (unsigned char)(cmd + 1);
    command->fail[0] = (unsigned char)(cmd + 2);
    command->nonce_err[0] = (unsigned char)NONCE;
    memset(command->data, 0, 2048);
    if (cmd == SENDKEY) {
        command->size = 512 + 64 + dbselect("select email from user").length();
        hex2str(command->data + 18, (unsigned char *)payload.toStdString().c_str(), command->size);
    }
    else {
        command->size = strlen(payload.toStdString().c_str());
        memcpy((char *)command->data + 18, payload.toStdString().c_str(), command->size);
        command->data[command->size + 18] = 0;
    }
    command->data[0] = command->code[0];
    command->data[17] = '\n';
    socket = new QTcpSocket(this);
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName("127.0.0.1");
    proxy.setPort(9250);
    QNetworkProxy::setApplicationProxy(proxy);
    socket->connectToHost(SERVER_HOST, SERVER_PORT);
    if (socket->waitForConnected(30000)) {
        if (handshake()== 1) finish(doAction());
        else finish(0);
        socket->close();
        delete socket;
    }
    else {
        qDebug() << "[*] Error:" << socket->errorString();
        finish(0);
        delete socket;
    }
    free(command);
    command = NULL;
    finish();
    return;
}

int Client::handshake() {
    qDebug() << "[*] Connected";
    int ret = 0;
    unsigned char *data = NULL;
    data = (unsigned char*)malloc(AES_KEYLEN/8 + AES_BLOCK_SIZE);
    if (data == NULL) goto con_err;
    memcpy(data, aes.getkey(), AES_KEYLEN/8);
    memcpy(data + AES_KEYLEN/8, aes.getiv(), AES_BLOCK_SIZE);
    rsa.setdata((char *)data, AES_KEYLEN/8 + AES_BLOCK_SIZE);
    ret = rsa.rsaEncrypt();
    if (ret == 0) goto con_err;
    if (socket->write((char *)rsa.getenc(), ret) != ret) goto con_err;
    rsa.freedata();
    rsa.freeenc();
    free(data);
    data = NULL;
    socket->waitForBytesWritten(10000);
    socket->waitForReadyRead(10000);
    data = (unsigned char*)malloc(512);
    ret = socket->read((char *)data, 512);
    qDebug() << "[*] Reading:" << ret;
    if (ret <= 0) goto con_err;
    aes.setenc((char *)data, ret);
    ret = aes.aesDecrypt();
    if (ret == 0) goto con_err;
    if (strncmp((char *)aes.getdec(), OK, sizeof(OK) - 1)==0) {
        memcpy(command->nonce, aes.getdec() + 2, 16);
        return 1;
    }
con_err:
    rsa.freedata();
    rsa.freeenc();
    if (data != NULL) free(data);
    data = NULL;
    return 0;
}

int Client::doAction() {
     int ret;
     memcpy((char *)command->data + 1, command->nonce, 16);
     aes.setdata((char *)command->data, command->size + 18);
     ret = aes.aesEncrypt();
     socket->write((char *)aes.getenc(), ret);
     socket->waitForBytesWritten(10000);
     socket->waitForReadyRead(10000);
     unsigned char* data;
     data = NULL;
     data = (unsigned char*)malloc(1024);
     if (data == NULL) return 0;
     ret = socket->read((char *)data, 1024);
     qDebug() << "[*] Reading:" << ret;
     if (ret < 1) {
         qDebug() << "[*] ERROR";
         if ((int)command->code[0] == SENDKEY) dbexec("update user set key_status='0'");
         free(data);
         data = NULL;
         return 0;
     }
     aes.setenc((char *)data, ret);
     ret = aes.aesDecrypt();
     free(data);
     data = NULL;
     if (aes.getdec()[0] == command->success[0]) {
        qDebug() << "[*] OK";
        if ((int)command->code[0] == SENDKEY) dbexec("update user set key_status='1'");
        if (((int)command->code[0] == LOGIN) || ((int)command->code[0] == ACTIVATE)) {
            data = NULL;
            data = (unsigned char*)malloc(64);
            if (data == NULL) return 0;
            QString token = "";
            str2hex((unsigned char*)data, aes.getdec()+1, 16);
            token.append((char*)data);
            dbexec(QString("update user set token='" + token + "'"));
            free(data);
            data = NULL;
        }
        if ((int)command->code[0] == NEWJOB) {
            QString str = "";
            str.append((char*)command->data + 18);
            ret = str.indexOf("\n") + 1;
            dbexec(QString("update jobs set received='1' where code='" + str.mid(ret, 16) + "'"));
        }
        if ((int)command->code[0] == SUBMITJOB) {
            QString str = "";
            str.append((char*)command->data + 18);
            ret = str.indexOf("\n") + 1;
            dbexec(QString("update jobs set submit='1' where code='" + str.mid(ret, 16) + "'"));
        }
        return (int)command->success[0];
     }
     else if (aes.getdec()[0] == command->fail[0]) {
        qDebug() << "[*] BAD";
        if ((int)command->code[0] == SENDKEY) dbexec("update user set key_status='0'");
        return (int)command->fail[0];
     }
     else if (aes.getdec()[0] == command->nonce_err[0]) {
        qDebug() << "[*] BAD NONCE";
        if ((int)command->code[0] == SENDKEY) dbexec("update user set key_status='0'");
        return 0;
     }
     return 0;
}

void Client::finish(int ret) {
    emit finished(ret);
}

void Client::finish() {
    emit finished();
}
