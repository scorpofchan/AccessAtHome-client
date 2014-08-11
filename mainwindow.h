#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "uploader.h"
#include "window.h"

#include <QDesktopWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Window *loginWindow;
    int status;

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

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
};

#endif // MAINWINDOW_H
