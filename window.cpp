#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Window)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":images/icon.gif"));
    status = 0;
    connect(ui->Logout, SIGNAL(triggered()), this, SLOT(doLogout()));
    connect(this, SIGNAL(doUpdate()), ui->tableView, SLOT(update()));
    model = new QStandardItemModel(dbselect("SELECT Count(*) FROM jobs").toInt(), 2, this);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Status"));
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setStyleSheet("selection-background-color: rgba(0, 0, 128, 10)");
    ui->tableView->horizontalHeader()->setHighlightSections(false);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->resizeColumnsToContents();
    Database *db = new Database;
    if (db->open()) {
        QSqlQuery query(QString("select name, status from jobs"));
        int index = 0;
        while (query.next()) {
            qDebug()<<"[*] "<<query.value(0).toString();
            model->setData(model->index(index, 0, QModelIndex()), query.value(0).toString());
            model->setData(model->index(index, 1, QModelIndex()), query.value(1).toString().compare("0") ? "Running" : "Not running");
            index++;
        }
        db->close();
    }
    delete db;
    db = NULL;
    ui->tableView->setModel(model);
    for (int i = 0; i < ui->tableView->horizontalHeader()->count(); ++i)
        ui->tableView->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
}

Window::~Window() {
    delete ui;
}

void Window::changeEvent(QEvent *e)
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

void Window::closeEvent(QCloseEvent *event) {
        hide();
        event->ignore();
}

void Window::on_infoButton_clicked() {
    if(ui->tableView->selectionModel()->hasSelection()) {
        int index = ui->tableView->selectionModel()->currentIndex().row();
        QString name(model->data(model->index(index, 0, QModelIndex())).toString());
        JobInfo *info = new JobInfo();
        info->setModal(true);
        info->setname(name);
        info->setWindowTitle(name);
        info->setdescription(dbselect(QString("select description from jobs where name='") + name + QString("'")));
        info->show();
    }
}

void Window::doLogout() {
    emit logout();
}

void Window::update() {
    qDebug()<<"[*] Update Job list";
    Database *db = new Database;
    if (db->open()) {
        QSqlQuery query(QString("select name, status from jobs"));
        int index = 0;
        model->removeRows(0,model->rowCount());
        while (query.next()) {
            model->insertRow(model->rowCount());
            model->setData(model->index(index, 0, QModelIndex()), query.value(0).toString());
            model->setData(model->index(index, 1, QModelIndex()), query.value(1).toString().compare("0") ? "Running" : "Not running");
            index++;
        }
        db->close();
    }
    delete db;
    db = NULL;
}
