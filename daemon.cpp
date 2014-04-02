#include "daemon.h"

Daemon::Daemon()
{
    torStatus = 0;
    listenStatus = 0;
}

void Daemon::start() {
    if(localPortIsOpen(QHostAddress::LocalHost,4444)) listenStatus = 0;
    else {
    //start listen
    Listener l;
    l.start();
    qDebug() << "starting Listener...";
    if(localPortIsOpen(QHostAddress::LocalHost,4444)) listenStatus = 1;
    if(listenStatus) qDebug() << "Listen started.";
    }
    if(localPortIsOpen(QHostAddress::LocalHost,9250) || localPortIsOpen(QHostAddress::LocalHost,9251)) torStatus = 0;
    else {
    // start tor
    qDebug() << "starting Tor...";
    //Py_SetProgramName(argv[0]);
    Py_Initialize();
    PyRun_SimpleString("from stem import process\n"
                       "process.launch_tor(tor_cmd='tor.exe', args=None, torrc_path='config', completion_percent=100, init_msg_handler=None, timeout=90, take_ownership=False)");
    Py_Finalize();
    //process("tor.exe");
    //execCmd("Tor\\tor.exe","-f Tor\\config",".");
    if(localPortIsOpen(QHostAddress::LocalHost,9250) && localPortIsOpen(QHostAddress::LocalHost,9251)) torStatus = 1;
    if(torStatus) qDebug() << "Tor started.";
    }
}
