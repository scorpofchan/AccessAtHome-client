#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client.h"
#include "uploader.h"
#include <time.h>
#include <QThread>
#include <QString>
#include <QDebug>
#include <QMainWindow>
#include <QDesktopWidget>
#include "window.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void updateRegister(int);
    void updateLogin(int);
    void updateActivate(int);
    void on_loginButton_clicked();
    void on_registerButton_clicked();
    void on_activateButton_clicked();
    void login();
    void logout();

private:
    Ui::MainWindow *ui;
    Window *loginWindow;
};

#endif // MAINWINDOW_H
