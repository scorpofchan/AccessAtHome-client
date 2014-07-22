#include "main.h"

JobsManager *jobsmanager = new JobsManager;

int init() {
    //listenStatus = 1;
    //if (localPortIsOpen(QHostAddress::LocalHost,9250) || localPortIsOpen(QHostAddress::LocalHost,9251)) torStatus = -1;
    //if (localPortIsOpen(QHostAddress::Any, 445)) listenStatus = 0;

    QThread *thread = new QThread;
    Daemon *Tor = new Daemon;
    Tor->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), Tor, SLOT(startTor()));
    thread->start();

    QThread *thread2 = new QThread;
    Daemon *daemon = new Daemon;
    daemon->moveToThread(thread2);
    QObject::connect(thread2, SIGNAL(started()), daemon, SLOT(startListener()));
    QObject::connect(daemon, SIGNAL(ListenerStarted()), daemon, SLOT(checkStatus()));
    QObject::connect(daemon, SIGNAL(finished()), thread2, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(thread2, SIGNAL(finished()), thread2, SLOT(deleteLater()));
    QObject::connect(thread2, SIGNAL(finished()), daemon, SLOT(deleteLater()));
    thread2->start();

    QThread *thread3 = new QThread;
    QObject::connect(thread3, SIGNAL(started()), jobsmanager, SLOT(worker()));
    jobsmanager->moveToThread(thread3);
    thread3->start();
    return 1;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //if (init()) qDebug()<<"[INIT] OK";
    //else qDebug()<<"[INIT] NO";
    Dialog *dialog = new Dialog();
    dialog->show();
    QTimer::singleShot(4000, dialog, SLOT(hide()));
    Systray *systray = new Systray();
    QObject::connect(systray, SIGNAL(exit()), &a, SLOT(quit()));
    QObject::connect(jobsmanager, SIGNAL(update()), systray, SLOT(update()));
    return a.exec();
}
