#ifndef MD5_HELPER_H
#define MD5_HELPER_H

#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

static inline void md5_hex(const char *input, char *out_hex) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
    EVP_DigestUpdate(ctx, input, strlen(input));
    EVP_DigestFinal_ex(ctx, digest, &digest_len);
    EVP_MD_CTX_free(ctx);
    for (unsigned int i = 0; i < digest_len; i++)
        sprintf(out_hex + (i * 2), "%02x", digest[i]);
    out_hex[32] = '\0';
}

#endif
