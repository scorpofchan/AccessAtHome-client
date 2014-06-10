#ifndef AESBOX_H
#define AESBOX_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#define AES_KEYLEN 256

class AESBox
{
public:
  AESBox();
  ~AESBox();
  int aesInit();
  int aesEncrypt();
  int aesDecrypt();
  int setdata(char *, unsigned int);
  int setenc(char *, unsigned int);
  int setkey(char *);
  int setiv(char *);
  void freectx();
  void freedata();
  void freeenc();
  void freedec();
  void freekey();
  void freeiv();
  unsigned char *getdata();
  unsigned char *getenc();
  unsigned char *getdec();
  unsigned char *getiv();
  unsigned char *getkey();

private:
  EVP_CIPHER_CTX *ctx;
  unsigned char *data;
  unsigned char *enc;
  unsigned char *dec;
  unsigned char *aesKey;
  unsigned char *aesIV;
  int dataLen;
  int encLen;
  int decLen;
};

#endif // AESBOX_H
