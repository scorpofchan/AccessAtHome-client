#ifndef GLOBAL_H
#define GLOBAL_H

#define NOMINMAX

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <tchar.h>

#include <QDebug>
#include <QTcpServer>

#define OK "OK"
#define HELLO "HELLO"
#define REGISTERcmd 1
#define LOGINcmd 2
#define REGISTER "REGISTER\n"
#define REGISTERSUCCEED "REGISTERSUCCEED"
#define REGISTERFAILED "REGISTERFAILED"
#define LOGIN "LOGIN\n"
#define LOGINSUCCEED "LOGINSUCCEED"
#define LOGINFAILED "LOGINFAILED"
#define NONCE "BADNONCE"
#define Error "ERROR"

#define REGISTER_SUCCEED 11
#define REGISTER_FAILED 12
#define LOGIN_SUCCEED 21
#define LOGIN_FAILED 22

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Kernel32.lib")

void process(char *);
void execCmd(char*, char*, char*);
void str2hex(unsigned char *, unsigned char *, int);
void hex2str(unsigned char *, unsigned char *, int);
int isEqual(unsigned char *, unsigned char *, int);
bool localPortIsOpen(QHostAddress, quint16);

static int torStatus = 0;
static int listenStatus = 0;
#endif // GLOBAL_H
