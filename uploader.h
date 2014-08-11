#ifndef UPLOADER_H
#define UPLOADER_H

#include "http.h"

class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QObject *parent = 0);
    Uploader::Uploader(QString, QString, QObject *parent = 0);

signals:
    void finished();

public slots:
    void genKey();
    void finish();
    void sendKey();
    void sendReport();

private:
    QString code;
    QString outputFile;

};

#endif // UPLOADER_H
