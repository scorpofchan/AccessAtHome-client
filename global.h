#ifndef GLOBAL_H
#define GLOBAL_H

#define NOMINMAX

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <windows.h>
#include <tchar.h>
#include "database.h"

#include <QDebug>
#include <QTcpServer>

#define OK "OK"
#define HELLO "HELLO"
#define REGISTER 10
#define LOGIN 20
#define ACTIVATE 30
#define INFO 40
#define SENDKEY 50
#define NEWJOB 200
#define REMOVEJOB 210
#define SUBMITJOB 220
#define REGISTER_SUCCEED 11
#define REGISTER_FAILED 12
#define LOGIN_SUCCEED 21
#define LOGIN_FAILED 22
#define ACTIVATE_SUCCEED 31
#define ACTIVATE_FAILED 32
#define INFO_SUCCEED 41
#define INFO_FAILED 42
#define SENDKEY_SUCCEED 51
#define SENDKEY_FAILED 52
#define NEWJOB_SUCCEED 201
#define NEWJOB_FAILED 202
#define REMOVEJOB_SUCCEED 211
#define REMOVEJOB_FAILED 212
#define SUBMITJOB_SUCCEED 221
#define SUBMITJOB_FAILED 221
#define NONCE 101

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Kernel32.lib")

void str2hex(unsigned char *, unsigned char *, int);
void hex2str(unsigned char *, unsigned char *, int);
int isEqual(unsigned char *, unsigned char *, int);
bool localPortIsOpen(QHostAddress, quint16);
int sha256(unsigned char *, char *, int);
int sha256_file(char *, unsigned char *);
int randbytes(unsigned char *, int);
bool setvalueDB(QString, QString, QString);
QString getvalueDB(QString, QString);
bool dbexec(QString);
QString dbselect(QString);
void addToStartup(char *);

static int torStatus = 0;
static int listenStatus = 0;

struct _command {
unsigned char code[1];
unsigned char nonce[16];
unsigned char data[2048];
unsigned char success[1];
unsigned char fail[1];
unsigned char nonce_err[1];
unsigned char error[1];
unsigned int size;
};

#endif // GLOBAL_H
