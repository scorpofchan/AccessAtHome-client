#ifndef DAEMON_H
#define DAEMON_H

#include "global.h"
#include "listener.h"

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

};

#endif // DAEMON_H
