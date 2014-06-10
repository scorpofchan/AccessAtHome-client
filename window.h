#ifndef WINDOW_H
#define WINDOW_H

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

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_clicked();

signals:
    void logout();

private:
    Ui::Window *ui;
    QStandardItemModel *model;
};

#endif // WINDOW_H
