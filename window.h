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

private slots:
    void on_pushButton_clicked();
    void doLogout();
    void update();

signals:
    void logout();

private:
    Ui::Window *ui;
    QStandardItemModel *model;
};

#endif // WINDOW_H
