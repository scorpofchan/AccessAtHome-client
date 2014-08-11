#include "job.h"

Job::Job(QString _code, QObject *parent) :
    QObject(parent), code(_code) {
    filename.append("jobs/" + _code + "/" + _code + ".json");
}

void Job::start() {
    QProcess process;
    QFile fp(filename);
    QJsonDocument jsonResponse;
    QJsonObject jsonObject;
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fp);
    jsonResponse = QJsonDocument::fromJson(QByteArray(in.readAll().toStdString().c_str()));
    fp.close();
    jsonObject = jsonResponse.object();
    QStringList args = jsonObject["arguments"].toString().split(" ");
    qDebug()<< "[*] Starting" <<jsonObject["file"].toString() << args;
    process.setWorkingDirectory(QString("jobs/" + code + "/" + code));
    process.start(QString("./jobs/" + code + "/" + code + "/" + jsonObject["file"].toString()), args);
    if (!process.waitForStarted()) {qDebug() << "[*] Failed"; return;};
    qDebug() << "Job started";
    process.waitForFinished(-1);
    qDebug()<<"[*] Job finished";
    if (dbexec(QString("update jobs set finished='1' where code='" + code + "'"))) submit();
}

void Job::check() {
    QFile fp(filename);
    QJsonDocument jsonResponse;
    QJsonObject jsonObject;
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fp);
    jsonResponse = QJsonDocument::fromJson(QByteArray(in.readAll().toStdString().c_str()));
    fp.close();
    jsonObject = jsonResponse.object();
    QString name(jsonObject["name"].toString());
    QString description(jsonObject["description"].toString());
    QString checksum(jsonObject["checksum"].toString());
    qDebug()<< name;
    qDebug()<< description;
    qDebug()<< checksum;
    dbexec(QString("update jobs set name='" + name + "', description='" + description
                   + "' where code='" + code + "'"));
    QString data = "";
    data = dbselect("select email from user");
    data.append("\n");
    data.append(code);
    data.append("\n");
    data.append(dbselect(QString("select token from jobs where code='" + code + "'")));
    QThread *thread = new QThread;
    Client *client = new Client(NEWJOB, data);
    Http *http = new Http(DOWNLOAD, URL_DOWNLOAD, "jobs/" + code + ".zip",
                          "jobs/" + code + "/" + code + ".zip");
    http->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), http, SLOT(downloadFile()));
    QObject::connect(http, SIGNAL(finished()), this, SLOT(unzip()));
    QObject::connect(http, SIGNAL(finished()), http, SLOT(deleteLater()));
    QObject::connect(this, SIGNAL(finished()), client, SLOT(start()));
    QObject::connect(client, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));
    //QObject::connect(thread, SIGNAL(finished()), this, SLOT(finish()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Job::submit() {
    QFile fp(filename);
    QJsonDocument jsonResponse;
    QJsonObject jsonObject;
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fp);
    jsonResponse = QJsonDocument::fromJson(QByteArray(in.readAll().toStdString().c_str()));
    fp.close();
    jsonObject = jsonResponse.object();
    QThread *thread = new QThread;
    Uploader *uploader = new Uploader(code, jsonObject["output"].toString());
    uploader->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), uploader, SLOT(sendReport()));
    QObject::connect(uploader, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), uploader, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), this, SLOT(finish()));
    thread->start();
}

int Job::checksum() {
    return 1;
}

void Job::unzip() {
    QProcess process;
    process.start(QString("zip.exe x jobs/" + code + "/" + code +".zip -ojobs/" + code));
    if (!process.waitForStarted()) return;
    qDebug() << "[*] Decompressing";
    process.waitForFinished(-1);
    emit finished();
}

void Job::finish() {
    emit finished();
}
