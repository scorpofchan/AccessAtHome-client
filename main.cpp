#include "main.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    //Daemon d;
    //d.start();
    MainWindow w;
    int width = 610;
    int height = 350;
    w.setFixedSize(width, height);
    w.setGeometry((QDesktopWidget().screen()->width()/2)-(width/2),(QDesktopWidget().screen()->height()/2)-(height/2),width,height);
    w.show();


    //w.setHidden(true);

    QString img;
    img="C:/Users/root/QtProjects/Core-client/icon.png";
    QPixmap p;
    if (!p.load(img) || p.isNull()) { qDebug() << "Error!"; }
    QIcon icon(img);
    QSystemTrayIcon t;
    t.setIcon(icon);
    t.show();
    //QString title="Name";
    //QString msg="message !!";
    //t.showMessage(title, msg);
    return a.exec();
}
