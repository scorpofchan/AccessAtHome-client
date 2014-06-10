#include "global.h"

void process(char *cmd){
       STARTUPINFOA si;
       PROCESS_INFORMATION pi;

       ZeroMemory( &si, sizeof(si) );
       si.cb = sizeof(si);
       ZeroMemory( &pi, sizeof(pi) );


       // Start the child process.
       if( !CreateProcessA(NULL,   // No module name (use command line)
           cmd,            // Command line
           NULL,           // Process handle not inheritable
           NULL,           // Thread handle not inheritable
           FALSE,          // Set handle inheritance to FALSE
           0,              // No creation flags
           NULL,           // Use parent's environment block
           NULL,           // Use parent's starting directory
           &si,            // Pointer to STARTUPINFO structure
           &pi )           // Pointer to PROCESS_INFORMATION structure
       )
       {
           printf( "CreateProcess failed (%d).\n", GetLastError() );
           return;
       }

       // Wait until child process exits.
       WaitForSingleObject( pi.hProcess,INFINITE);

       // Close process and thread handles.
       CloseHandle( pi.hProcess );
       CloseHandle( pi.hThread );
}

void execCmd(char *file, char *args, char *dir) {
    //SetEnvironmentVariableA(reinterpret_cast<LPCSTR>(""), reinterpret_cast<LPCSTR>(""));
    int nRet = (int)ShellExecuteA( 0,
                                  reinterpret_cast<LPCSTR>("open"),
                                  reinterpret_cast<LPCSTR>(file),
                                  reinterpret_cast<LPCSTR>(args),
                                  reinterpret_cast<LPCSTR>(dir),
                                  SW_HIDE);
    if ( nRet <= 32 ) {
                qDebug() << nRet <<"Erorr !!";
    }
}

void str2hex(unsigned char *dst, unsigned char *s, int len) {
        for (int i = 0; i < len; i++) _snprintf((char *)&dst[2*i], 3, "%02x", s[i]);
        dst[len*2] = 0;
}

void hex2str(unsigned char *dst, unsigned char *s, int len) {
        int c;
        char tmp[3];
        for (int i = 0; i < len; i++) {
            tmp[0] = s[2*i];
            tmp[1] = s[2*i + 1];
            tmp[2] = 0;
            c = strtoul(tmp, NULL, 16);
            dst[i] = c;
        }
        dst[len] = 0;
}

int isEqual(unsigned char *p1, unsigned char *p2, int len) {
    int status = 1;
    for (int i = 0; i < len; i++) {
        if ((status == 1) && (*(p1 + i) != *(p2 + i))) status = 0;
    }
   return status;
}

bool localPortIsOpen(QHostAddress address, quint16 port) {
     QTcpServer *server = new QTcpServer();
     if (!server->listen(address, port)) return true;
     else return false;
     server->close();
     delete server;
}

int sha256(unsigned char *buffer, char *str, int len) {
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, len);
    SHA256_Final(buffer, &sha256);
    return 1;
}

int sha256_file(char *path, unsigned char *buffer) {
    FILE *file = NULL;
    fopen_s(&file, path, "rb");
    if(!file) return 0;
    unsigned char *buf;
    int size = 8192;
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    buf = NULL;
    buf = (unsigned char *)malloc(size);
    if(buf == NULL) return 0;
    int nread = 0;
    while((nread = fread(buf, 1, size, file))) SHA256_Update(&sha256, buf, nread);
    SHA256_Final(buffer, &sha256);
    fclose(file);
    file = NULL;
    free(buf);
    buf = NULL;
    return 1;
}

int randbytes(unsigned char *key, int len) {
    if (RAND_bytes(key, len) == 0) return 0;
    return 1;
}

bool setvalueDB(QString field, QString value, QString table) {
    bool ret = false;
    Database *db = new Database;
    if (!db->open()) return ret;
    if (!QString::compare(table,"jobs")) {
        qDebug()<<"ggggggg";
        qDebug()<<field<<value<<table;
        QSqlQuery query;
        query.exec(QString("insert into jobs(id) values(null)"));
    }
    ret = db->setvalue(field, value, table);
    db->close();
    delete db;
    db = NULL;
    return ret;
}

QString getvalueDB(QString field, QString table) {
    QString ret = "";
    Database *db = new Database;
    if (!db->open()) return ret;
    ret = db->getvalue(field, table);
    db->close();
    delete db;
    db = NULL;
    return ret;
}

bool dbexec(QString str) {
    bool ret = false;
    Database *db = new Database;
    if (!db->open()) return ret;
    QSqlQuery query;
    ret = query.exec(str);
    db->close();
    delete db;
    db = NULL;
    return ret;
}

QString dbselect(QString str) {
    QString ret = "";
    Database *db = new Database;
    if (!db->open()) return ret;
    QSqlQuery query(str);
    if (query.next()) ret = query.value(0).toString();
    db->close();
    delete db;
    db = NULL;
    return ret;
}
