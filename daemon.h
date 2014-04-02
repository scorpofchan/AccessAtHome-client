#ifndef DAEMON_H
#define DAEMON_H

#include "global.h"
#include "listener.h"
#include <Python.h>

#include <QProcess>

class Daemon
{
public:
    Daemon();
    void start();
};

#endif // DAEMON_H
