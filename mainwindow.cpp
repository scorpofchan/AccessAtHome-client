#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete c;
    delete ui;
}

void MainWindow::on_loginButton_clicked() {
    if((ui->loginEmail->text() != "") && (ui->loginPass->text() != "")) {
      ui->loginButton->setCheckable(true);
      QString data = "";
      int status;
      data.append(ui->loginEmail->text());
      data.append("\n");
      data.append(ui->loginPass->text());
      c = new Client();
      status = c->start(LOGINcmd, data);
      switch (status) {
      case LOGIN_SUCCEED:
          ui->statusLogin->setText("Login succeed");
          break;
      case LOGIN_FAILED:
          ui->statusLogin->setText("Login failed");
          break;
      default:
          ui->statusLogin->setText("Error");
          break;
      }
      Sleep(2000);
      ui->loginButton->setCheckable(false);
    }
}

void MainWindow::on_registerButton_clicked() {
    if((ui->registerEmail->text() != "") && (ui->registerPass->text() != "")
            && (ui->registerPass2->text() != "")) {
      QString data = "";
      int status;
      data.append(ui->registerEmail->text());
      data.append("\n");
      data.append(ui->registerPass->text());
      c = new Client();
      status = c->start(REGISTERcmd, data);
      switch (status) {
      case REGISTER_SUCCEED:
          ui->statusRegister->setText("Register succeed");
          break;
      case REGISTER_FAILED:
          ui->statusRegister->setText("Register failed");
          break;
      default:
          ui->statusRegister->setText("Error");
          break;
      }
    }
}
