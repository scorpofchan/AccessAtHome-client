#ifndef SYSTRAY_H
#define SYSTRAY_H

#include "mainwindow.h"
#include <QMenu>
#include <QTimer>
#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>

class Systray : public QObject
{
    Q_OBJECT
public:
    explicit Systray(QObject *parent = 0);
    void Systray::setIcon(QString);
    void createActions();
    void createTrayMenu();

signals:
    void exit();

public slots:
    void update();
    void quit();
    void showNormal();
    void activate(QSystemTrayIcon::ActivationReason);

private:
    MainWindow mainwindow;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // SYSTRAY_H
