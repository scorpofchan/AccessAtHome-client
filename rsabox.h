#ifndef RSABOX_H
#define RSABOX_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#define RSA_KEY_LEN 4096
#define MAX_LEN RSA_KEY_LEN/16
#define PRIVATEKEY "keys/private.key"
#define PUBLICKEY "keys/server.key"
#define MYPUBLICKEY "keys/public.key"

class RSABox
{
public:
  RSABox();
  ~RSABox();
  int genRsaKey(char *, char *);
  int rsaEncrypt();
  int rsaDecrypt();
  int rsaSign();
  int rsaVerify();
  int setPrivKey(char *);
  int setPubKey(char *);
  int setdata(char *, unsigned int);
  int setenc(char *);
  int setsig(char *);
  void freepkey();
  void freectx();
  void freeprivkey();
  void freepubkey();
  void freedata();
  void freeenc();
  void freedec();
  void freesig();
  unsigned char *getdata();
  unsigned char *getenc();
  unsigned char *getdec();
  unsigned char *getsig();

private:
  EVP_PKEY_CTX *ctx;
  EVP_PKEY *pkey;
  EVP_PKEY *pubkey;
  EVP_PKEY *privkey;
  unsigned char *data;
  unsigned char *enc;
  unsigned char *dec;
  unsigned char *sig;
  size_t dataLen;
  size_t encLen;
  size_t decLen;
  size_t sigLen;
};

#endif // RSABOX_H
