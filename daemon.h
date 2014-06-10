#ifndef DAEMON_H
#define DAEMON_H

#include "global.h"
#include "listener.h"
#include "uploader.h"
#include <QThread>
#include <QObject>
#include <QProcess>
#include <QFileInfo>
#include <QDir>

class Daemon : public QObject
{
    Q_OBJECT
public:
    explicit Daemon(QObject *parent = 0);

signals:
    void finished(int);
    void finished();
    void TorStarted();
    void ListenerStarted();

public slots:
    void startListener();
    void startTor();
    void init();
    void checkStatus();
    void finish(int);
    void finish();
    void hello();

};

#endif // DAEMON_H
