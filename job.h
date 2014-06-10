#ifndef JOB_H
#define JOB_H

#include <QString>
#include <Qdebug>
#include <QObject>
#include "http.h"

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QString, QObject *parent = 0);
    int checksum();

signals:
    void finished(QString);
    void finished();

public slots:
    void start();
    void check();
    void finish();

private:
    QString code;
    QString filename;
};

#endif // JOB_H
