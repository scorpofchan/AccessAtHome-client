#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":images/icon.gif"));
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    status = 0;
    QTimer *timer = new QTimer(this);
    timer->start(4000);
    for (int i = 0; i < 20; i++)
    timer->singleShot(i * 500, this, SLOT(updateStatus()));
}

void Dialog::closeEvent(QCloseEvent *event) {
        hide();
        event->ignore();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::updateStatus() {
     int k = status % 4;
     switch(k) {
     case 0:
         ui->label->setText("Starting Access@Home, please wait");
         break;
     case 1:
         ui->label->setText("Starting Access@Home, please wait .");
         break;
     case 2:
         ui->label->setText("Starting Access@Home, please wait ..");
         break;
     case 3:
         ui->label->setText("Starting Access@Home, please wait ...");
         break;
     default:
         break;
     }

     status++;
}
