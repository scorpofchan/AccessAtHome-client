#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QCloseEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void updateStatus();

private:
    Ui::Dialog *ui;
    int status;
};

#endif // DIALOG_H
