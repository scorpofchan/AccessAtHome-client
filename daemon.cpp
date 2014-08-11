#include "daemon.h"

Daemon::Daemon(QObject *parent) :
    QObject(parent) {

}

void Daemon::startListener() {
    qDebug() << "[*] Starting Listener...";
    QThread *thread = new QThread;
    Listener *listener = new Listener;
    listener->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), listener, SLOT(start()));
    thread->start();
    emit ListenerStarted();
}

void Daemon::startTor() {
    QProcess process;
    if (!QDir("tor/hidden_service").exists()) QDir().mkdir("tor/hidden_service");
    process.start("tor/tor.exe", QStringList() << "-f" << "tor/config");
    if (!process.waitForStarted()) return;
    qDebug() << "[*] Starting Tor...";
    emit TorStarted();
    process.waitForFinished(-1);
    qDebug()<<"[*] Tor Stopped";
    emit finished();
}

void Daemon::init() {
    qDebug()<<"[*] Starting...";
    if (!QDir("keys").exists()) QDir().mkdir("keys");
    if (!QDir("jobs").exists()) QDir().mkdir("jobs");
    //Database *db = new Database;
    //if (!db->open()) return;
    //db->createTables();
    //db->close();
    //delete db;
    //db = NULL;
    QThread::sleep(10);
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
        dbexec("update user set key='1'");
        if ((dbselect("select email from user") != "") && (dbselect("select key_status from user") != "1")) {
            qDebug()<<"[SENDKEY]";
            QThread *thread2 = new QThread;
            Uploader *uploader = new Uploader;
            uploader->moveToThread(thread2);
            QObject::connect(thread2, SIGNAL(started()), uploader, SLOT(sendKey()));
            QObject::connect(uploader, SIGNAL(finished()), thread2, SLOT(quit()), Qt::DirectConnection);
            QObject::connect(thread2, SIGNAL(finished()), thread2, SLOT(deleteLater()));
            QObject::connect(thread2, SIGNAL(finished()), uploader, SLOT(deleteLater()));
            thread2->start();
        }
    }
    else {
        dbexec("update user set key='0'");
        qDebug()<<"[GENKEY]";
        QThread *thread3 = new QThread;
        Uploader *uploader2 = new Uploader;
        uploader2->moveToThread(thread3);
        QObject::connect(thread3, SIGNAL(started()), uploader2, SLOT(genKey()));
        QObject::connect(uploader2, SIGNAL(finished()), thread3, SLOT(quit()), Qt::DirectConnection);
        QObject::connect(thread3, SIGNAL(finished()), uploader2, SLOT(deleteLater()));
        QObject::connect(thread3, SIGNAL(finished()), thread3, SLOT(deleteLater()));
        thread3->start();
    }
    if (dbselect("select email from user") != "") {
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
