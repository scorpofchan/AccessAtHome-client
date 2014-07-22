#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), loginWindow(new Window)
{
    ui->setupUi(this);
    status = 0;
    setWindowIcon(QIcon(":images/icon.gif"));
    //loginWindow->setFixedSize(width, height);
    //connect(ui->loginButton,SIGNAL(clicked()),this,SLOT(login()));
    connect(loginWindow,SIGNAL(logout()),this,SLOT(logout()));
}

MainWindow::~MainWindow()
{
    delete loginWindow;
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
        hide();
        event->ignore();
}


void MainWindow::login() {
    this->hide();
    status = 0;
    int width = 310;
    int height = 440;
    loginWindow->setFixedSize(width, height);
    loginWindow->setGeometry((QDesktopWidget().screen()->width()/2)-(width/2),(QDesktopWidget().screen()->height()/2)-(height/2),width,height);
    loginWindow->show();
    loginWindow->status = 1;
}

void MainWindow::logout() {
    loginWindow->hide();
    loginWindow->status = 0;
    int width = 310;
    int height = 380;
    setGeometry((QDesktopWidget().screen()->width()/2)-(width/2),(QDesktopWidget().screen()->height()/2)-(height/2),width,height);
    this->show();
    status = 1;
}

void MainWindow::updateLogin(int status) {
    switch (status) {
      case LOGIN_SUCCEED:
        //ui->statusLogin->setText("Login succeed");
        login();
        break;
      case LOGIN_FAILED:
        ui->statusLogin->setText("Login failed");
        break;
      default:
        ui->statusLogin->setText("Error");
        break;
    }
    ui->loginEmail->clear();
    ui->loginPass->clear();
    ui->loginButton->setDisabled(false);
    ui->loginButton->setEnabled(true);
}

void MainWindow::updateActivate(int status) {
    QThread *thread;
    QThread *thread2;
    Uploader *uploader;
    Http *http;
    switch (status) {
      case ACTIVATE_SUCCEED:
        ui->statusActivate->setText("Activation succeed");
        setvalueDB("email", ui->activateEmail->text(), "user");
        thread = new QThread;
        uploader = new Uploader;
        uploader->moveToThread(thread);
        connect(thread, SIGNAL(started()), uploader, SLOT(sendkey()));
        connect(uploader, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), uploader, SLOT(deleteLater()));
        thread->start();
        QThread::sleep(2);
        thread2 = new QThread;
        http = new Http(IP_INFO);
        http->moveToThread(thread2);
        QObject::connect(thread2, SIGNAL(started()), http, SLOT(sendInfo()));
        QObject::connect(http, SIGNAL(finished()), thread2, SLOT(quit()), Qt::DirectConnection);
        QObject::connect(thread2, SIGNAL(finished()), thread2, SLOT(deleteLater()));
        QObject::connect(thread2, SIGNAL(finished()), http, SLOT(deleteLater()));
        thread2->start();
        break;
      case ACTIVATE_FAILED:
        ui->statusActivate->setText("Activation failed");
        break;
      default:
        ui->statusActivate->setText("Error");
        break;
    }
    ui->activateEmail->clear();
    ui->activateCode->clear();
    ui->activateButton->setDisabled(false);
    ui->activateButton->setEnabled(true);
}

void MainWindow::updateRegister(int status) {
    switch (status) {
      case REGISTER_SUCCEED:
         ui->statusRegister->setText("");
         ui->activateEmail->setText(ui->registerEmail->text());
         ui->tab->setCurrentWidget(ui->Activate);
         ui->statusActivate->setText("Please check you email");
         break;
      case REGISTER_FAILED:
         ui->statusRegister->setText("Register failed");
         break;
      default:
         ui->statusRegister->setText("Error");
         break;
    }
    ui->registerEmail->clear();
    ui->registerPass->clear();
    ui->registerPass2->clear();
    ui->registerButton->setDisabled(false);
    ui->registerButton->setEnabled(true);
}

void MainWindow::on_loginButton_clicked() {
    login();
    if ((ui->loginEmail->text() != "") && (ui->loginPass->text() != "")) {
      if ((ui->loginPass->text().size() > 30) || (ui->loginEmail->text().size() > 30)) {
          ui->statusLogin->setText("Too long information");
          return;
      }
      ui->loginButton->setDisabled(true);
      ui->loginButton->setEnabled(false);
      ui->statusLogin->setText("Waiting . . .");
      QString data = "";
      data.append(ui->loginEmail->text());
      data.append("\n");
      data.append(ui->loginPass->text());
      QThread *thread = new QThread();
      Client *client = new Client(LOGIN, data);
      client->moveToThread(thread);
      connect(thread, SIGNAL(started()), client, SLOT(start()));
      connect(client, SIGNAL(finished(int)), this, SLOT(updateLogin(int)));
      connect(client, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
      connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
      connect(thread, SIGNAL(finished()), client, SLOT(deleteLater()));
      thread->start();
    }
}

void MainWindow::on_registerButton_clicked() {
    if((ui->registerEmail->text() != "") && (ui->registerPass->text() != "")
            && (ui->registerPass2->text() != "")) {
      if ((ui->registerPass->text().size() > 30) || (ui->registerPass2->text().size() > 30)
              || (ui->registerEmail->text().size() > 30)) {
          ui->statusRegister->setText("Too long information");
          return;
      }
      if (ui->registerPass->text() != ui->registerPass2->text()) {
          ui->statusRegister->setText("Passwords didn't match");
          return;
      }
      ui->registerButton->setDisabled(true);
      ui->registerButton->setEnabled(false);
      ui->statusRegister->setText("Waiting . . .");
      QString data = "";
      data.append(ui->registerEmail->text());
      data.append("\n");
      data.append(ui->registerPass->text());
      QThread *thread = new QThread();
      Client *client = new Client(REGISTER, data);
      client->moveToThread(thread);
      connect(thread, SIGNAL(started()), client, SLOT(start()));
      connect(client, SIGNAL(finished(int)), this, SLOT(updateRegister(int)));
      connect(client, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
      connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
      connect(thread, SIGNAL(finished()), client, SLOT(deleteLater()));
      thread->start();
    }
}

void MainWindow::on_activateButton_clicked() {
    if((ui->activateEmail->text() != "") && (ui->activateCode->text() != "")) {
      if ((ui->activateEmail->text().size() > 30) || (ui->activateCode->text().size() != 16)) {
          ui->statusActivate->setText("Bad information");
          return;
      }
      ui->activateButton->setDisabled(true);
      ui->activateButton->setEnabled(false);
      ui->statusActivate->setText("Waiting . . .");
      QString data = "";
      data.append(ui->activateEmail->text());
      data.append("\n");
      data.append(ui->activateCode->text());
      QThread *thread = new QThread();
      Client *client = new Client(ACTIVATE, data);
      client->moveToThread(thread);
      connect(thread, SIGNAL(started()), client, SLOT(start()));
      connect(client, SIGNAL(finished(int)), this, SLOT(updateActivate(int)));
      connect(client, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
      connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
      connect(thread, SIGNAL(finished()), client, SLOT(deleteLater()));
      thread->start();
    }
}
