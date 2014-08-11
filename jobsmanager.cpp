#include "jobsmanager.h"

JobsManager::JobsManager(QObject *parent) : QObject(parent){}

void JobsManager::worker() {
    QThread::sleep(5);
    Database *db1 = new Database;
    if (db1->open()) {
        QSqlQuery query(QString("select code from jobs where received='1' and submit='0' and finished='1'"));
        while (query.next()) {
            qDebug()<<"[*] Submit job!";
            QThread *thread = new QThread;
            Job *job = new Job(query.value(0).toString());
            job->moveToThread(thread);
            QObject::connect(thread, SIGNAL(started()), job, SLOT(submit()));
            QObject::connect(job, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
            QObject::connect(thread, SIGNAL(finished()), job, SLOT(deleteLater()));
            QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            thread->start();
        }
        db1->close();
    }
    delete db1;
    db1 = NULL;
    while (true) {
        QThread::sleep(5);
        QQueue<QString> jobs;
        Database *db = new Database;
        if (db->open()) {
            QSqlQuery query(QString("select code from jobs where received='1' and submit='0' and finished='0'"));
            while (query.next()) {
                jobs.enqueue(query.value(0).toString());
            }
            db->close();
        }
        delete db;
        db = NULL;
        while (!jobs.isEmpty()) {
            QString code = jobs.dequeue();
            Job *job = new Job(code);
            if (dbexec(QString("update jobs set status=1 where code='" + code + "'"))) emit update();
            qDebug()<<"[*] Running job!";
            job->start();
            if (dbexec(QString("update jobs set status=0 where code='" + code + "'"))) emit update();
            delete job;
            job = NULL;
        }
    }
}
