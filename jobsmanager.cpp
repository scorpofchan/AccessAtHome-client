#include "jobsmanager.h"

JobsManager::JobsManager(QObject *parent) :
    QObject(parent)
{
}

void JobsManager::worker() {
    while (true) {
        QThread::sleep(5);
        Database *db = new Database;
        if (!db->open()) return;
        QSqlQuery query(QString("select code from jobs where received='1' and submit='0' and finished='0'"));
        QQueue<QString> jobs;
        while (query.next()) {
            jobs.enqueue(query.value(0).toString());
        }
        db->close();
        delete db;
        db = NULL;
        while (!jobs.isEmpty()) {
            QString code = jobs.dequeue();
            Job *job = new Job(code);
            dbexec(QString("update jobs set status=1 where code='" + code + "'"));
            emit update();
            job->start();
            dbexec(QString("update jobs set status=0 where code='" + code + "'"));
            delete job;
            job = NULL;
        }
    }
}
