#ifndef WINDOW_H
#define WINDOW_H

#include "global.h"
#include "jobinfo.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class Window;
}

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();
    int status;

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

public slots:
    void on_infoButton_clicked();
    void doLogout();
    void update();

signals:
    void logout();
    void doUpdate();

private:
    Ui::Window *ui;
    QStandardItemModel *model;
};

#endif // WINDOW_H
