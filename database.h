#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class Database
{
public:
        static bool open();
        static void close();
        static void setDB();
        static bool createTables();

        static QSqlDatabase *db;
};


#endif // DATABASE_H
