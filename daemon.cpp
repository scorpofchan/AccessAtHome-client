#include "daemon.h"

Daemon::Daemon(QObject *parent) :
    QObject(parent) {

}

void Daemon::startListener() {
    if (listenStatus == 0) {
    qDebug() << "starting Listener...";
    QThread *thread = new QThread;
    Listener *listener = new Listener;
    listener->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), listener, SLOT(start()));
    thread->start();
    }
    emit ListenerStarted();
}

void Daemon::startTor() {
    QProcess process;
    process.start("tor/tor.exe", QStringList() << "-f" << "tor/config");
    if (!process.waitForStarted()) return;
    qDebug() << "starting Tor...";
    process.waitForFinished(-1);
    qDebug()<<"[*] Tor Stopped";
    emit TorStarted();
}

void Daemon::checkStatus() {

    //Please check this logic.

    qDebug() << "[*] Checking...";
    QThread::sleep(10);
    if (localPortIsOpen(QHostAddress::LocalHost,4444)
        && localPortIsOpen(QHostAddress::LocalHost,9250)
        && localPortIsOpen(QHostAddress::LocalHost,9251))
    { qDebug()<<"[*] OK"; init();}
    else {
        qDebug()<<"[*] NO";
        init();
        //emit finished();
    }
}

void Daemon::init() {
    qDebug()<<"[*] Starting...";
    Database *db = new Database;
    if (!db->open()) return;
    if (!db->createTable()) return;
    db->close();
    delete db;
    db = NULL;
    if (!QFileInfo(PUBLICKEY).isFile()) {
        qDebug()<<"[DOWNLOAD KEY]";
        QThread *thread = new QThread();
        Http *http = new Http(DOWNLOAD, URL_DOWNLOAD, "key.txt", PUBLICKEY);
        http->moveToThread(thread);
        QObject::connect(thread, SIGNAL(started()), http, SLOT(downloadFile()));
        QObject::connect(http, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
        QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        QObject::connect(thread, SIGNAL(finished()), http, SLOT(deleteLater()));
        thread->start();
    }
    if (QFileInfo(MYPUBLICKEY).isFile()) {
        setvalueDB("key", "1", "user");
        if ((getvalueDB("email", "user") != "") && (getvalueDB("key_status", "user") != "1")) {
            qDebug()<<"[SENDKEY]";
            QThread *thread2 = new QThread;
            Uploader *uploader = new Uploader;
            uploader->moveToThread(thread2);
            QObject::connect(thread2, SIGNAL(started()), uploader, SLOT(sendkey()));
            QObject::connect(uploader, SIGNAL(finished()), thread2, SLOT(quit()), Qt::DirectConnection);
            QObject::connect(thread2, SIGNAL(finished()), thread2, SLOT(deleteLater()));
            QObject::connect(thread2, SIGNAL(finished()), uploader, SLOT(deleteLater()));
            thread2->start();
        }
    }
    else {
        setvalueDB("key", "0", "user");
        if (!QDir("keys").exists()) QDir().mkdir("keys");
        qDebug()<<"[GENKEY]";
        QThread *thread3 = new QThread;
        Client *client = new Client;
        client->moveToThread(thread3);
        QObject::connect(thread3, SIGNAL(started()), client, SLOT(genkey()));
        QObject::connect(client, SIGNAL(finished()), thread3, SLOT(quit()), Qt::DirectConnection);
        QObject::connect(thread3, SIGNAL(finished()), thread3, SLOT(deleteLater()));
        QObject::connect(thread3, SIGNAL(finished()), client, SLOT(deleteLater()));
        thread3->start();
    }
    if (getvalueDB("email", "user") != "") {
        qDebug()<<"[SEND INFO]";
        QThread *thread4 = new QThread;
        Http *http = new Http(IP_INFO);
        http->moveToThread(thread4);
        QObject::connect(thread4, SIGNAL(started()), http, SLOT(sendInfo()));
        QObject::connect(http, SIGNAL(finished()), thread4, SLOT(quit()), Qt::DirectConnection);
        QObject::connect(thread4, SIGNAL(finished()), thread4, SLOT(deleteLater()));
        QObject::connect(thread4, SIGNAL(finished()), http, SLOT(deleteLater()));
        thread4->start();
    }
    emit finished();
}

void Daemon::finish(int ret) {
    emit finished(ret);
}

void Daemon::finish() {
    emit finished();
}

void Daemon::hello() {
    qDebug()<<"hello";
}
