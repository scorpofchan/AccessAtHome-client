#include "jobinfo.h"
#include "ui_jobinfo.h"

JobInfo::JobInfo(QWidget *parent) :
    QDialog(parent), ui(new Ui::JobInfo)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":images/icon.gif"));
    setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->descritpion->setReadOnly(true);
}

void JobInfo::setname(QString name) {
    ui->name->setText(name);
}

void JobInfo::setdescription(QString desc) {
    ui->descritpion->setText(desc);
}

JobInfo::~JobInfo() {
    delete ui;
}

void JobInfo::on_closeButton_clicked() {
    this->close();
}

void JobInfo::on_nextButton_clicked() {
}
