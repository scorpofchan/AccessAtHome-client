#include "jobinfo.h"
#include "ui_jobinfo.h"

JobInfo::JobInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JobInfo)
{
    ui->setupUi(this);
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowIcon(QIcon(":images/icon.gif"));
}

void JobInfo::setname(QString name) {
    ui->label_2->setText(name);
}

void JobInfo::setdescription(QString desc) {
    ui->textEdit->setText(desc);
    ui->textEdit->setReadOnly(true);
}

JobInfo::~JobInfo()
{
    delete ui;
}
