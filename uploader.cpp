#include "uploader.h"

Uploader::Uploader(QObject *parent) : QObject(parent) {}

Uploader::Uploader(QString _code, QString _outputFile, QObject *parent) : QObject(parent) {
    code =_code;
    outputFile =_outputFile;
}

void Uploader::genKey() {
     RSABox *rsa = new RSABox;
     if (rsa->genRsaKey(PRIVATEKEY, MYPUBLICKEY)) dbexec("update user set key='1'");
     delete rsa;
     emit finished();
}

void Uploader::sendKey() {
   QString email(dbselect("select email from user"));
   if ((email != "") && (dbselect("select key_status from user") != "1")) {
    unsigned char *tmp = NULL;
    unsigned char *rand = NULL;
    unsigned char *checksum = NULL;
    tmp = (unsigned char *)malloc(2048);
    rand = (unsigned char *)malloc(65);
    checksum = (unsigned char*)malloc(32);
    if ((tmp == NULL) || (rand == NULL) || (checksum == NULL)) {
        if (tmp != NULL) free(tmp);
        if (rand != NULL) free(rand);
        if (checksum != NULL) free(checksum);
        tmp = NULL;
        rand = NULL;
        checksum = NULL;
        return ;
    }
    randbytes(tmp, 32);
    str2hex(rand, tmp, 32);
    qDebug() << "[*]" << QString::fromUtf8((char *)rand);
    str2hex(tmp, rand, 64);
    sha256_file(MYPUBLICKEY, checksum);
    int ret = 0;
    RSABox rsa;
    rsa.setPrivKey(PRIVATEKEY);
    rsa.setdata((char *)checksum, 32);
    ret = rsa.rsaSign();
    if (ret == 0) return;
    str2hex(tmp + 128, rsa.getsig(), ret);
    str2hex(tmp + 128 + 1024, (unsigned char*)email.toStdString().c_str(), email.length());
    QString data = "";
    data.append(QString::fromUtf8((char *)tmp, (512 + 64 + email.length())*2));
    QThread *thread = new QThread();
    Http *http = new Http(UPLOAD, URL_UPLOAD, MYPUBLICKEY, QString::fromUtf8((char *)rand, 64));
    Client *client = new Client(SENDKEY, data);
    free(tmp);
    free(rand);
    free(checksum);
    checksum = rand = tmp = NULL;
    http->moveToThread(thread);
    connect(thread, SIGNAL(started()), http, SLOT(uploadFile()));
    connect(http, SIGNAL(finished()), client, SLOT(start()));
    connect(client, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), client, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), http, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(finish()));
    thread->start();
   }
}

void Uploader::sendReport() {
    qDebug()<< "[*] SEND REPORT";
    QString data = "";
    QString token(dbselect(QString("select token from jobs where code='" + code + "'")));
    data = dbselect("select email from user");
    data.append("\n");
    data.append(code);
    data.append("\n");
    data.append(token);
    QThread *thread = new QThread();
    Client *client = new Client(SUBMITJOB, data);
    Http *http = new Http(UPLOAD, URL_UPLOAD, QString("jobs/" + code + "/" + code + "/" + outputFile), token);
    http->moveToThread(thread);
    connect(thread, SIGNAL(started()), http, SLOT(uploadFile()));
    connect(http, SIGNAL(finished()), client, SLOT(start()));
    connect(client, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), client, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), http, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(finish()));
    thread->start();
}

void Uploader::finish() {
   emit finished();
}
