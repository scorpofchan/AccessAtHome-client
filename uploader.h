#ifndef UPLOADER_H
#define UPLOADER_H

#include "http.h"
#include "client.h"
#include <QObject>
#include <QDebug>


class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QObject *parent = 0);

signals:
    void finished();

public slots:
    void sendkey();
    void finish();
    void hello();
private:

};

#endif // UPLOADER_H
