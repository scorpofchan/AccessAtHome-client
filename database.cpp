#include "database.h"

QSqlDatabase* Database::db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));

void Database::setDB() {
    db->setDatabaseName("db.sqlite");
}

bool Database::open() {
    qDebug()<<"[*] ============================================> open1 DB";
    //db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    qDebug()<<"[*] ============================================> open2 DB";
    db->setDatabaseName("db.sqlite");
    if(db->isOpen()) return true;
    else return db->open();
}

void Database::close() {
    qDebug()<<"[*] ============================================> close1 DB";
    //db->close();
    //QString name(db->connectionName());
    //delete db;
    //db = NULL;
    //QSqlDatabase::removeDatabase(name);
    qDebug()<<"[*] ============================================> close2 DB";
}

bool Database::createTables() {
    bool ret;
    QSqlQuery query;

    ret = query.exec("create table if not exists user "
                  "(id integer primary key, "
                  "email varchar(40) default '', "
                  "token varchar(100) default '', "
                  "key varchar(2) default '', "
                  "key_status varchar(2) default '')");
    ret = query.exec("create table if not exists jobs "
                  "(id integer primary key, "
                  "name varchar(40) default '', "
                  "description varchar(256) default '', "
                  "code varchar(24) default '', "
                  "token varchar(100) default '', "
                  "status varchar(2) default '0', "
                  "received varchar(2) default '0', "
                  "finished varchar(2) default '0', "
                  "submit varchar(2) default '0')");
    QSqlQuery query2("select id from user");
    if (!query2.next()) query.exec(QString("insert into user(id) values(null)"));
    return ret;
}
