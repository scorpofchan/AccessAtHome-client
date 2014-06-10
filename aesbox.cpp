#include "aesbox.h"

AESBox::AESBox() {
  data = enc = dec = aesKey = aesIV = NULL;
  dataLen = encLen = decLen = 0;
  ctx = NULL;
}
int AESBox::aesInit() {
  if (aesKey != NULL) freekey();
  if (aesIV != NULL) freeiv();
  aesKey = (unsigned char*)malloc(AES_KEYLEN/8);
  aesIV = (unsigned char*)malloc(AES_BLOCK_SIZE);
  if (aesKey == NULL || aesIV == NULL) return 0;
  if (RAND_bytes(aesKey, AES_KEYLEN/8) == 0) return 0;
  if (RAND_bytes(aesIV, AES_BLOCK_SIZE) == 0) return 0;
  return 1;
}

int AESBox::aesEncrypt() {
  int blockLen  = 0;
  if (enc != NULL) freeenc();
  if (enc == NULL) enc = (unsigned char*)malloc(dataLen + AES_BLOCK_SIZE);
  if (enc == NULL) goto enc_err;
  if (ctx != NULL) freectx();
  ctx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
  if (ctx == NULL) goto enc_err;
  EVP_CIPHER_CTX_init(ctx);
  if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aesKey, aesIV)) goto enc_err;
  if (!EVP_EncryptUpdate(ctx, enc, &blockLen, data, dataLen)) goto enc_err;
  encLen += blockLen;
  if (!EVP_EncryptFinal_ex(ctx, enc + encLen, &blockLen)) goto enc_err;
  encLen += blockLen;
  freectx();
  return encLen;
enc_err:
  freectx();
  return 0;
}

int AESBox::aesDecrypt() {
  int blockLen = 0;
  if (dec != NULL) freedec();
  if (dec == NULL) dec = (unsigned char*)malloc(encLen);
  if (dec == NULL) goto dec_err;
  if (ctx != NULL) freectx();
  ctx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
  if (ctx == NULL) goto dec_err;
  EVP_CIPHER_CTX_init(ctx);
  if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aesKey, aesIV)) goto dec_err;
  if (!EVP_DecryptUpdate(ctx, dec, &blockLen, enc, encLen)) goto dec_err;
  decLen += blockLen;
  if (!EVP_DecryptFinal_ex(ctx, dec + decLen, &blockLen)) goto dec_err;
  decLen += blockLen;
  freectx();
  return decLen;
dec_err:
  freectx();
  return 0;
}

int AESBox::setdata(char *s, unsigned int n) {
  if (data != NULL) freedata();
  data = (unsigned char *)malloc(n);
  if (data == NULL) return 0;
  for (unsigned int i = 0; i < n; i++) data[i] = (unsigned char)s[i];
  dataLen = n;
  return 1;
}

int AESBox::setenc(char *s, unsigned int n) {
  if (enc != NULL) freeenc();
  enc = (unsigned char *)malloc(n);
  if (enc == NULL) return 0;
  for (unsigned int i = 0; i < n; i++) enc[i] = (unsigned char)s[i];
  encLen = n;
  return 1;
}

int AESBox::setkey(char *s) {
  if (aesKey != NULL) freekey();
  aesKey = (unsigned char*)malloc(AES_KEYLEN/8);
  if (aesKey == NULL) return 0;
  for (unsigned int i = 0; i < AES_KEYLEN/8; i++) aesKey[i] = (unsigned char)s[i];
  return 1;
}

int AESBox::setiv(char *s) {
  if (aesIV != NULL) freekey();
  aesIV = (unsigned char*)malloc(AES_BLOCK_SIZE);
  if (aesIV == NULL) return 0;
  for (unsigned int i = 0; i < AES_BLOCK_SIZE; i++) aesIV[i] = (unsigned char)s[i];
  return 1;
}

void AESBox::freedata() {
  dataLen = 0;
  if (data != NULL) free(data);
  data = NULL;
}

void AESBox::freeenc() {
  encLen = 0;
  if (enc != NULL) free(enc);
  enc = NULL;
}

void AESBox::freedec() {
  decLen = 0;
  if (dec != NULL) free(dec);
  dec = NULL;
}

void AESBox::freekey() {
  if (aesKey != NULL) free(aesKey);
  aesKey = NULL;
}

void AESBox::freeiv() {
  if (aesIV != NULL) free(aesIV);
  aesIV = NULL;
}

void AESBox::freectx() {
  if (ctx != NULL) {EVP_CIPHER_CTX_cleanup(ctx); free(ctx);}
  ctx = NULL;
}

unsigned char* AESBox::getdata() {
  return data;
}

unsigned char* AESBox::getenc() {
  return enc;
}

unsigned char* AESBox::getdec() {
  return dec;
}

unsigned char* AESBox::getkey() {
  return aesKey;
}

unsigned char* AESBox::getiv() {
  return aesIV;
}

AESBox::~AESBox() {
  void freectx();
  void freedata();
  void freeenc();
  void freedec();
  void freekey();
  void freeiv();
}
