#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Database
{
public:
        //~Database();
        bool open();
        void close();
        bool createTable();
        QString getvalue(QString, QString);
        bool setvalue(QString, QString, QString);
        QSqlError lastError();

private:
        QSqlDatabase *db;
};

#endif // DATABASE_H
