#ifndef JOBSMANAGER_H
#define JOBSMANAGER_H

#include "job.h"
#include "database.h"
#include <QObject>
#include <QQueue>

class JobsManager : public QObject
{
    Q_OBJECT
public:
    explicit JobsManager(QObject *parent = 0);

signals:
    void update();

public slots:
    void worker();

};

#endif // JOBSMANAGER_H
