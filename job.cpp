#include "job.h"

Job::Job(QString _code, QObject *parent) :
    QObject(parent), code(_code) {
    filename.append("jobs/" + _code + "/" + _code + ".json");
}

void Job::start() {

}

void Job::check() {
    QFile fp(filename);
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fp);
    QJsonDocument jsonResponse;
    jsonResponse = QJsonDocument::fromJson(QByteArray(in.readAll().toStdString().c_str()));
    fp.close();
    QJsonObject jsonObject;
    jsonObject = jsonResponse.object();
    QVariant name(jsonObject["name"].toString());
    QVariant description(jsonObject["description"].toString());
    QVariant file(jsonObject["file"].toString());
    QVariant checksum(jsonObject["checksum"].toString());
    qDebug()<< name.toString();
    qDebug()<< description.toString();
    qDebug()<< file.toString();
    qDebug()<< checksum.toString();
    dbexec(QString("update jobs set name='" + name.toString() + "', description='" + description.toString()
                   + "' where code='" + code + "'"));
    QString data = "";
    data = getvalueDB("email", "user");
    data.append("\n");
    data.append(code);
    data.append("\n");
    data.append(dbselect(QString("select token from jobs where code='" + code + "'")));
    QThread *thread = new QThread;
    Client *client = new Client(NEWJOB, data);
    Http *http = new Http(DOWNLOAD, URL_DOWNLOAD, "jobs/" + file.toString(),
                           "jobs/" + code + "/" + file.toString());
    http->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), http, SLOT(downloadFile()));
    QObject::connect(http, SIGNAL(finished()), client, SLOT(start()));
    QObject::connect(http, SIGNAL(finished()), http, SLOT(deleteLater()));
    QObject::connect(client, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), this, SLOT(finish()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

int Job::checksum() {
    return 1;
}

void Job::finish() {
    emit finished();
}
