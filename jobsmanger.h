#ifndef JOBSMANGER_H
#define JOBSMANGER_H

#include <QObject>

class JobsManger : public QObject
{
    Q_OBJECT
public:
    explicit JobsManger(QObject *parent = 0);

signals:

public slots:
    void JobsManger::a();
    void JobsManger::b();

};

#endif // JOBSMANGER_H
