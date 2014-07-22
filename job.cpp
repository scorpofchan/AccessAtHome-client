#include "job.h"

Job::Job(QString _code, QObject *parent) :
    QObject(parent), code(_code) {
    filename.append("jobs/" + _code + "/" + _code + ".json");
}

void Job::start() {
    QObject::connect(this, SIGNAL(send()), this, SLOT(submit()));
    QFile fp(filename);
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fp);
    QJsonDocument jsonResponse;
    jsonResponse = QJsonDocument::fromJson(QByteArray(in.readAll().toStdString().c_str()));
    fp.close();
    QJsonObject jsonObject;
    jsonObject = jsonResponse.object();
    qDebug()<< "[*] Starting" <<jsonObject["file"].toString() << "...";
    QProcess process;
    process.setWorkingDirectory(QString("jobs/" + code + "/" + code));
    process.start(QString("./jobs/" + code + "/" + code + "/" + jsonObject["file"].toString()));
    if (!process.waitForStarted()) {qDebug() << "[*] Failed"; return;};
    qDebug() << "Job started";
    process.waitForFinished(-1);
    qDebug()<<"[*] Job finished";
    dbexec(QString("update jobs set finished='1' where code='" + code + "'"));
    emit send();
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
    QString name(jsonObject["name"].toString());
    QString description(jsonObject["description"].toString());
    QString checksum(jsonObject["checksum"].toString());
    qDebug()<< name;
    qDebug()<< description;
    qDebug()<< checksum;
    dbexec(QString("update jobs set name='" + name + "', description='" + description
                   + "' where code='" + code + "'"));
    QString data = "";
    data = getvalueDB("email", "user");
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
    QObject::connect(thread, SIGNAL(finished()), this, SLOT(finish()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void Job::submit() {
    QFile fp(filename);
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fp);
    QJsonDocument jsonResponse;
    jsonResponse = QJsonDocument::fromJson(QByteArray(in.readAll().toStdString().c_str()));
    fp.close();
    QJsonObject jsonObject;
    jsonObject = jsonResponse.object();
    QString output(jsonObject["output"].toString());
    QThread *thread = new QThread;
    Uploader *uploader = new Uploader(code, output);
    uploader->moveToThread(thread);
    QObject::connect(thread, SIGNAL(started()), uploader, SLOT(sendreport()));
    QObject::connect(uploader, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), uploader, SLOT(deleteLater()));
    thread->start();
}

int Job::checksum() {
    return 1;
}

void Job::unzip() {
    QProcess process;
    process.start(QString("zip.exe x jobs/" + code + "/" + code +".zip -ojobs/" + code));
    if (!process.waitForStarted()) {qDebug() << "[*] Failed"; return;};
    qDebug() << "[*] UNZIP started";
    process.waitForFinished(-1);
    qDebug()<<"[*] UNZIP finished";
    emit finished();
}

void Job::finish() {
    emit finished();
}
