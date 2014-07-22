#include "handler.h"


Handler::Handler(qintptr sock, QObject *parent) :
    QThread(parent) {
    this->socketDescriptor = sock;
}

void Handler::run() {
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor)) {
        emit error(socket->error());
        socket->close();
        emit close();
        return;
    }
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this, SIGNAL(close()), socket, SLOT(deleteLater()));
    if(socket->waitForReadyRead(10000)) {
        qDebug() << "["<<socketDescriptor << "] Connected";
        requestHandler();
    }
    exec();
}

void Handler::requestHandler() {
    command = NULL;
    command = (_command *)malloc(sizeof(_command));
    if (command == NULL) return;
    command->nonce_err[0] = (unsigned char)NONCE;
    handshake();
    free(command);
    command = NULL;
}

int Handler::handshake() {
    qDebug()<<"[*] Handshake";
    int ret;
    unsigned char *ack;
    ack = NULL;
    ret = rsa.setPrivKey((char *)PRIVATEKEY);
    if (ret == 0) goto hs_err;
    char *data = NULL;
    data = (char*)malloc(512);
    ret = 0;
    int nloop = 0;
    while ((ret < 512) && (nloop < 12)) {
        int nbytes = 0;
        nbytes = socket->read((char *)(data + ret), 512 - ret);
        ret += nbytes;
        qDebug()<<"[*] ret : "<<ret;
        if (ret == 512) break;
        socket->waitForReadyRead(10000);
        nloop++;
    }
    rsa.setenc(data);
    ret = rsa.rsaDecrypt();
    if ((ret == 0) || (ret != (AES_KEYLEN/8 + AES_BLOCK_SIZE))) goto hs_err;
    aes.setkey((char *)rsa.getdec());
    aes.setiv((char *)rsa.getdec() + AES_KEYLEN/8);
    ack = (unsigned char*)malloc(18);
    if (ack == NULL) goto hs_err;
    ret = randbytes(ack + 2, 16);
    ack[0] = 'O'; ack[1] = 'K';
    if (ret == 0) goto hs_err;
    aes.setdata((char *)ack, 18);
    ret = aes.aesEncrypt();
    if (ret == 0) goto hs_err;
    memcpy(command->nonce, ack + 2, 16);
    //if (socket->write((char*)aes.getenc(), ret) != ret) goto hs_err;
    socket->write((char*)aes.getenc(), ret);
    socket->waitForBytesWritten(10000);
    unsigned char *buffer;
    buffer = NULL;
    buffer = (unsigned char*)malloc(1024);
    if (buffer == NULL) goto hs_err;
    socket->waitForReadyRead(10000);
    ret = 0;
    nloop = 0;
    while ((ret < 80) && (nloop < 12)) {
        int nbytes = 0;
        nbytes = socket->read((char *)(buffer + ret), 1024 - ret);
        ret += nbytes;
        qDebug()<<"[*] ret : "<<ret;
        if (ret == 80) break;
        socket->waitForReadyRead(10000);
        nloop++;
    }
    //ret = socket->read((char *)buffer, 1024);
    qDebug() <<"[ Nread = " << ret << "]";
    if (ret < 1) socket->close();
    handler(buffer, ret);
    rsa.freeprivkey();
    rsa.freeenc();
    rsa.freedec();
    aes.freedata();
    aes.freeenc();
    free(ack);
    ack = NULL;
    free(buffer);
    buffer = NULL;
    emit finish();
    return 1;
hs_err:
    if (ack != NULL) {free(ack); ack = NULL;}
    rsa.freeprivkey();
    rsa.freeenc();
    rsa.freedec();
    aes.freekey();
    aes.freeiv();
    aes.freedata();
    aes.freeenc();
    emit finish();
    return 0;
}

void Handler::handler(unsigned char * buffer, int ret) {
    qDebug()<<"[*] Start Handler";
    aes.setenc((char *)buffer, ret);
    ret = aes.aesDecrypt();
    qDebug() << "[LEN] " << ret;
    if ((ret < 18) && (ret < 2048)) {
        aes.freeenc();
        aes.freedec();
        socket->close();
        emit finish();
        return;
    }
    command->code[0] = aes.getdec()[0];
    command->success[0] = aes.getdec()[0] + 1;
    command->fail[0] = aes.getdec()[0] + 2;
    command->size = ret - 18;
    qDebug()<< "[CODE] " << (int)command->code[0];
    if (isEqual(command->nonce, aes.getdec() + 1, 16)) {
        qDebug() << "[OK] NONCE";
        memcpy(command->data, aes.getdec() + 18, ret - 18);
        command->data[ret - 18] = 0;
    }
    else {
        qDebug() << "[NO] NONCE";
        aes.setdata((char *)command->nonce_err, 1);
        ret = aes.aesEncrypt();
        if (ret != 0) {
            if (socket->write((char*)aes.getenc(), ret) == ret) {
                aes.freeenc();
                aes.freedec();
                socket->close();
                emit finish();
                return;
            }
            else {
                aes.freeenc();
                aes.freedec();
                socket->close();
                emit finish();
                return;
            }
        }
    }
    switch ((int) command->code[0]) {
            case NEWJOB:
              newjobHandler();

              break;
            default:
              //printf("[FREEEE]\n");
              //free(command);
              //command = NULL;
              socket->close();
              emit finish();
              return;
              break;
    }
}

void Handler::newjobHandler() {
    qDebug() << "[*] NEWJOB";
    int ret = 0;
    QString data = "";
    data.append((char *)command->data);
    qDebug()<<"[*] "<<data.mid(0,16);
    qDebug()<<"[*] "<<data.mid(16,48);
    if (dbselect(QString("select code from jobs where code='" + data.mid(0,16) + "'")).compare(data.mid(0,16)) == 0) {
        qDebug()<< "UPDATE JOB";
        dbexec(QString("update jobs set token='" + data.mid(16) + "' where code='" + data.mid(0,16) + "'"));
    }
    else {
        qDebug()<< "INSERT JOB";
        dbexec(QString("insert into jobs(code, token) values('" + data.mid(0,16) + "','" + data.mid(16) + "')"));
    }
    aes.setdata((char *)command->success, 1);
    ret = aes.aesEncrypt();
    if (ret != 0) {
        ret = socket->write((char*)aes.getenc(), ret);
        socket->waitForBytesWritten(10000);
    }
    aes.freedata();
    aes.freeenc();
    socket->close();
    QDir().mkdir(QString("jobs/" + data.mid(0,16)));
    qDebug()<<"[DOWNLOAD JOB]";
    QThread *thread = new QThread;
    Http *http = new Http(DOWNLOAD, URL_DOWNLOAD, "jobs/" + data.mid(0,16) + ".json",
                          "jobs/" + data.mid(0,16) + "/" + data.mid(0,16) + ".json");
    Job *job = new Job(data.mid(0,16));
    http->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), http, SLOT(downloadFile()));
    QObject::connect(http, SIGNAL(finished()), job, SLOT(check()));
    QObject::connect(job, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(thread, SIGNAL(finished()), job, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    //emit finish();
}

void Handler::hello() {
    qDebug() << "[*] HELLO";
    int ret = 0;
    QString s;
    s.append((char *)command->data);
    qDebug()<<s;
    aes.setdata((char *)command->success, 1);
    ret = aes.aesEncrypt();
    if (ret != 0) {
        ret = socket->write((char*)aes.getenc(), ret);
        socket->waitForBytesWritten(10000);
    }
    aes.freedata();
    aes.freeenc();
    socket->close();
    //emit finish();
}

void Handler::disconnected() {
    qDebug() << "[" <<socketDescriptor << "] Disconnected";
    socket->deleteLater();
    emit finish();
}
