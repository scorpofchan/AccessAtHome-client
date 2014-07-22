#include "systray.h"

Systray::Systray(QObject *parent) :
    QObject(parent) {
    trayIcon = new QSystemTrayIcon(this);
    setIcon(":images/icon.gif");
    createActions();
    createTrayMenu();
    trayIcon->show();
    trayIcon->showMessage("Welcome!", "Access@Home started.", QSystemTrayIcon::Information, 3000);


    //int width = 600;
    //int height = 340;
    int width = 310;
    int height = 380;
    mainwindow.setFixedSize(width, height);
    mainwindow.setGeometry((QDesktopWidget().screen()->width()/2)-(width/2),(QDesktopWidget().screen()->height()/2)-(height/2),width,height);
    QTimer::singleShot(4000, &mainwindow, SLOT(show()));
    mainwindow.status = 1;
    //mainwindow.show();
    //mainwindow.setHidden(true);
}

void Systray::setIcon(QString img) {
    trayIcon->setIcon(QIcon(img));
}

void Systray::createActions() {
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activate(QSystemTrayIcon::ActivationReason)));
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), &mainwindow, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), &mainwindow, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
}

void Systray::createTrayMenu() {
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
}

void Systray::quit() {
emit exit();
}

void Systray::showNormal() {
   if(mainwindow.status == 1) mainwindow.showNormal();
   else if(mainwindow.loginWindow->status == 1) mainwindow.loginWindow->showNormal();
}

void Systray::update() {
   qDebug()<<"updateeee!";
}

void Systray::activate(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::ActivationReason::DoubleClick)
        showNormal();
}
