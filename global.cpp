#include "global.h"

void execCmd(char *file, char *args, char *dir)
{
    SetEnvironmentVariableA(reinterpret_cast<LPCSTR>(""),
                            reinterpret_cast<LPCSTR>(""));
    int nRet= (int)ShellExecuteA( 0,
                                  reinterpret_cast<LPCSTR>("open"),
                                  reinterpret_cast<LPCSTR>(file),
                                  reinterpret_cast<LPCSTR>(args),
                                  reinterpret_cast<LPCSTR>(dir),
                                  SW_HIDE);
    if ( nRet <= 32 ) {
                qDebug() << "Erorr !!";
    }
    qDebug() << nRet;
}
