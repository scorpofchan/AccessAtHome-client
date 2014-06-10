#include "main.h"

int init(){
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
    return 1;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    /*Job aa("fwUNzfzuHPoZTu1s");
    aa.check();
    exit(0);

    /*
    //QMessageBox::information(0, "Try Again", "Please try to activate your Bluetooth again.");
    QThread *thread = new QThread;
    MsgBox *msg = new MsgBox;
    //msg->moveToThread(thread);
    msg->moveToThread( QApplication::instance()->thread() );
    QCoreApplication::postEvent( msg, new QEvent( QEvent::User ) );
    //QObject::connect(thread, SIGNAL(started()), msg, SLOT(DisplayMessageBox()));
    //thread->start();
    */
    if (init()) qDebug()<<"[INIT] OK";
    else qDebug()<<"[INIT] NO";
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle("Starting");
    box->setText("Please waiting...");
    box->setStandardButtons(QMessageBox::NoButton);
    box->show();
    QTimer::singleShot(1000, box, SLOT(hide()));


/*

    QThread *thread = new QThread;
    Uploader *uploader = new Uploader;
    uploader->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), uploader, SLOT(sendkey()));
    QObject::connect(uploader, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), uploader, SLOT(deleteLater()));
    thread->start();*/

    //Daemon d;
    //d.start();
    MainWindow w;
    int width = 610;
    int height = 350;
    w.setFixedSize(width, height);
    w.setGeometry((QDesktopWidget().screen()->width()/2)-(width/2),(QDesktopWidget().screen()->height()/2)-(height/2),width,height);
    QTimer::singleShot(1000, &w, SLOT(show()));
    //w.show();
    //w.setHidden(true);
    /*
    QString img;
    img=":img/icon.png";
    QPixmap p;
    if (!p.load(img) || p.isNull()) { qDebug() << "Error!"; }
    QIcon icon(img);
    QSystemTrayIcon t;
    t.setIcon(icon);
    t.show();
    QString title="ACCESS@HOME";
    QString msg="Welcome !!";
    t.showMessage(title, msg);*/
    return a.exec();
}
