#include "rsabox.h"
#include <QDebug>

RSABox::RSABox() {
    ctx = NULL;
    pkey = privkey = pubkey = NULL;
    data = enc = dec = sig = NULL;
    dataLen = encLen = decLen = sigLen = 0;
}

int RSABox::genRsaKey() {
    if (ctx != NULL) freectx();
    if (pkey != NULL) freepkey();
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) goto gen_err;
    if ((EVP_PKEY_keygen_init(ctx)) <= 0) goto gen_err;
    if ((EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, RSA_KEY_LEN)) <= 0) goto gen_err;
    if ((EVP_PKEY_keygen(ctx, &pkey)) <= 0) goto gen_err;
    freectx();
    return 1;
gen_err:
    freectx();
    freepkey();
    return 0;
}

int RSABox::setPrivKey() {
    FILE *f = NULL;
    fopen_s(&f, "private.key", "r");
    if (f) {
      if(privkey != NULL) freeprivkey();
      privkey = PEM_read_PrivateKey(f, &privkey, 0, 0);
      fclose(f);
      f = NULL;
      if(privkey != NULL) return 1;
    }
    return 0;
}

int RSABox::setPubKey() {
      FILE *f = NULL;
      fopen_s(&f, "public.key", "r");
      if (f) {
        if(pubkey != NULL) freepubkey();
        if(!PEM_read_PUBKEY(f, &pubkey, NULL, NULL))
        fclose(f);
        f = NULL;
        if(pubkey != NULL) return 1;
      }
      return 0;
}

int RSABox::rsaEncrypt() {
    if (ctx != NULL) freectx();
    if (pubkey == NULL) goto enc_err;
    ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx) goto enc_err;
    if (EVP_PKEY_encrypt_init(ctx) <= 0) goto enc_err;
    if ((EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING)) <= 0) goto enc_err;
    if ((EVP_PKEY_encrypt(ctx, NULL, &encLen, data, dataLen)) <= 0) goto enc_err;
    if (enc == NULL) enc = (unsigned char *)malloc(encLen);
    if (enc == NULL) goto enc_err;
    if ((EVP_PKEY_encrypt(ctx, enc, &encLen, data, dataLen)) <= 0) {
       freeenc();
       goto enc_err;
    }
    freectx();
    return encLen;
enc_err:
    freectx();
    return 0;
}

int RSABox::rsaDecrypt() {
    if (ctx != NULL) freectx();
    if (privkey == NULL) goto dec_err;
    ctx = EVP_PKEY_CTX_new(privkey, NULL);
    if (!ctx) goto dec_err;
    if ((EVP_PKEY_decrypt_init(ctx)) <= 0) goto dec_err;
    if ((EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING)) <= 0) goto dec_err;
    if ((EVP_PKEY_decrypt(ctx, NULL, &decLen, enc, encLen)) <= 0) goto dec_err;
    if (dec == NULL) dec = (unsigned char *)malloc(decLen);
    if (dec == NULL) goto dec_err;
    if ((EVP_PKEY_decrypt(ctx, dec, &decLen, enc, encLen)) <= 0) {
       freedec();
       goto dec_err;
    }
    freectx();
    return decLen;
dec_err:
    freectx();
    return 0;
}

int RSABox::rsaSign() {
    if (ctx != NULL) freectx();
    if (privkey == NULL) goto sig_err;
    ctx = EVP_PKEY_CTX_new(privkey, NULL);
    if (!ctx) goto sig_err;
    if ((EVP_PKEY_sign_init(ctx)) <= 0) goto sig_err;
    if ((EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING)) <= 0) goto sig_err;
    //if ((EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256())) <= 0) goto sig_err;
    if ((EVP_PKEY_sign(ctx, NULL, &sigLen, data, dataLen)) <= 0) goto sig_err;
    if (sig == NULL) sig = (unsigned char *)malloc(sigLen);
    if (sig == NULL) goto sig_err;
    if ((EVP_PKEY_sign(ctx, sig, &sigLen, data, dataLen)) <= 0) {
       freesig();
       goto sig_err;
    }
    freectx();
    return sigLen;
sig_err:
    freectx();
    return 0;
}

int RSABox::rsaVerify() {
    if (ctx != NULL) freectx();
    if (pubkey == NULL) goto ver_err;
    ctx = EVP_PKEY_CTX_new(pubkey, NULL);
    if (!ctx) goto ver_err;
    if ((EVP_PKEY_verify_init(ctx)) <= 0) goto ver_err;
    if ((EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING)) <= 0) goto ver_err;
    //if ((EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256())) <= 0) goto ver_err;
    if ((EVP_PKEY_verify(ctx, sig, sigLen, data, dataLen)) != 1) goto ver_err;
    freectx();
    return 1;
ver_err:
    freectx();
    return 0;
}

int RSABox::setdata(char *s, unsigned int n) {
    if (n > MAX_LEN || n < 1) return 0;
    if (data != NULL) freedata();
    if (data == NULL) data = (unsigned char *)malloc(n);
    dataLen = 0;
    if (data == NULL) return 0;
    for (unsigned int i = 0; i < n; i++) data[i] = (unsigned char)s[i];
    dataLen = (size_t) n;
    return 1;
}

int RSABox::setenc(char *s) {
    unsigned int n = RSA_KEY_LEN/8;
    if (enc != NULL) freeenc();
    if (enc == NULL) enc = (unsigned char *)malloc(n);
    encLen = 0;
    if (enc == NULL) return 0;
    for (unsigned int i = 0; i < n; i++) enc[i] = (unsigned char)s[i];
    encLen = (size_t) n;
    return 1;
}

int RSABox::setsig(char *s) {
    unsigned int n = RSA_KEY_LEN/8;
    if (sig != NULL) freesig();
    if (sig == NULL) sig = (unsigned char *)malloc(n);
    sigLen = 0;
    if (sig == NULL) return 0;
    for (unsigned int i = 0; i < n; i++) sig[i] = (unsigned char)s[i];
    sigLen = (size_t) n;
    return 1;
}

void RSABox::freedata() {
    dataLen = 0;
    if (data != NULL) free(data);
    data = NULL;
}

void RSABox::freeenc() {
    encLen = 0;
    if (enc != NULL) free(enc);
    enc = NULL;
}

void RSABox::freedec() {
    decLen = 0;
    if (dec != NULL) free(dec);
    dec = NULL;
}

void RSABox::freesig() {
    sigLen = 0;
    if (sig != NULL) free(sig);
    sig = NULL;
}

void RSABox::freepkey() {
    if(pkey != NULL) EVP_PKEY_free(pkey);
    pkey = NULL;
}

void RSABox::freepubkey() {
    if(pubkey != NULL) EVP_PKEY_free(pubkey);
    pubkey = NULL;
}

void RSABox::freeprivkey() {
    if(privkey != NULL) EVP_PKEY_free(privkey);
    privkey = NULL;
}

void RSABox::freectx() {
    if(ctx != NULL) EVP_PKEY_CTX_free(ctx);
    ctx = NULL;
}

unsigned char* RSABox::getdata() {
        return data;
}

unsigned char* RSABox::getenc() {
        return enc;
}

unsigned char* RSABox::getdec() {
        return dec;
}

unsigned char* RSABox::getsig() {
        return sig;
}

RSABox::~RSABox() {
        freedata();
        freeenc();
        freedec();
        freesig();
        freepkey();
        freeprivkey();
        freepubkey();
        freectx();
}
