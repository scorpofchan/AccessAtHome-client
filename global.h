#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include <windows.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Kernel32.lib")

void execCmd(char*, char*, char*);

#endif // GLOBAL_H
