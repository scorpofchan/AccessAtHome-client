#ifndef JOB_H
#define JOB_H

#include "uploader.h"
#include <QProcess>

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(QString, QObject *parent = 0);
    int checksum();

signals:
    void finished();
    void send();

public slots:
    void start();
    void check();
    void unzip();
    void submit();
    void finish();

private:
    QString code;
    QString filename;
};

#endif // JOB_H
