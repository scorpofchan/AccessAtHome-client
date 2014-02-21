#include <QCoreApplication>
#include "listener.h"
#include "downloader.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Listener s;
    s.start();
    Downloader d;
    d.doDownload();
    return a.exec();
}
