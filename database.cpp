#include "database.h"

bool Database::open() {
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    //*db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName("db.sqlite");
    return db->open();
}

QSqlError Database::lastError() {
    return db->lastError();
}

void Database::close() {
    db->close();
    QString name(db->connectionName());
    delete db;
    db = NULL;
    QSqlDatabase::removeDatabase(name);

}

bool Database::createTable() {
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
    if (getvalue("id", "user") != "1") query.exec(QString("insert into user(id) values(null)"));
    return ret;
}

/*
bool Database::insert(QString email, QString token) {
    bool ret = false;
    QSqlQuery query(QString("select * from user where email = '%1'").arg(email));
    if (query.next()) {
        ret = query.exec(QString("update user set token = '%1' where email = '%2'")
          .arg(token).arg(email));
    }
    else {
        ret = query.exec(QString("insert into user(email, token) values('%1','%2')")
          .arg(email).arg(token));
    }
    return ret;
}
*/

QString Database::getvalue(QString field, QString table) {
    QSqlQuery query(QString("select " + field + " from " + table + " where id = 1"));
    if (query.next()) return query.value(0).toString();
    else return QString("");
}

bool Database::setvalue(QString field, QString value, QString table) {
    return QSqlQuery().exec(QString("update " + table + " set " + field + " = '%1' where id = 1")
                           .arg(value));
}

/*bool Database::select(int id,  person) {
    bool ret = false;
    QSqlQuery query(QString("select * from person where id = %1").arg(id));
    if (query.next())
        {
        person->id = query.value(0).toInt();
        person->firstname = query.value(1).toString();
        person->lastname = query.value(2).toString();
        person->age = query.value(3).toInt();
        ret = true;
        }
    return ret;
}*/
/*
Database::~Database(){
        QSqlDatabase::removeDatabase(db.connectionName());
}
*/
