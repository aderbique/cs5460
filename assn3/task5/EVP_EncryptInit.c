#include <openssl/evp.h>

 void EVP_CIPHER_CTX_init(EVP_CIPHER_CTX *a);

 int EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
         ENGINE *impl, unsigned char *key, unsigned char *iv);
 int EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out,
         int *outl, unsigned char *in, int inl);
 int EVP_EncryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *out,
         int *outl);

 int EVP_DecryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
         ENGINE *impl, unsigned char *key, unsigned char *iv);
 int EVP_DecryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out,
         int *outl, unsigned char *in, int inl);
 int EVP_DecryptFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *outm,
         int *outl);

 int EVP_CipherInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
         ENGINE *impl, unsigned char *key, unsigned char *iv, int enc);
 int EVP_CipherUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out,
         int *outl, unsigned char *in, int inl);
 int EVP_CipherFinal_ex(EVP_CIPHER_CTX *ctx, unsigned char *outm,
         int *outl);

 int EVP_EncryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
         unsigned char *key, unsigned char *iv);
 int EVP_EncryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *out,
         int *outl);

 int EVP_DecryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
         unsigned char *key, unsigned char *iv);
 int EVP_DecryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm,
         int *outl);

 int EVP_CipherInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
         unsigned char *key, unsigned char *iv, int enc);
 int EVP_CipherFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm,
         int *outl);

 int EVP_CIPHER_CTX_set_padding(EVP_CIPHER_CTX *x, int padding);
 int EVP_CIPHER_CTX_set_key_length(EVP_CIPHER_CTX *x, int keylen);
 int EVP_CIPHER_CTX_ctrl(EVP_CIPHER_CTX *ctx, int type, int arg, void *ptr);
 int EVP_CIPHER_CTX_cleanup(EVP_CIPHER_CTX *a);

 const EVP_CIPHER *EVP_get_cipherbyname(const char *name);
 #define EVP_get_cipherbynid(a) EVP_get_cipherbyname(OBJ_nid2sn(a))
 #define EVP_get_cipherbyobj(a) EVP_get_cipherbynid(OBJ_obj2nid(a))

 #define EVP_CIPHER_nid(e)              ((e)->nid)
 #define EVP_CIPHER_block_size(e)       ((e)->block_size)
 #define EVP_CIPHER_key_length(e)       ((e)->key_len)
 #define EVP_CIPHER_iv_length(e)                ((e)->iv_len)
 #define EVP_CIPHER_flags(e)            ((e)->flags)
 #define EVP_CIPHER_mode(e)             ((e)->flags) & EVP_CIPH_MODE)
 int EVP_CIPHER_type(const EVP_CIPHER *ctx);

 #define EVP_CIPHER_CTX_cipher(e)       ((e)->cipher)
 #define EVP_CIPHER_CTX_nid(e)          ((e)->cipher->nid)
 #define EVP_CIPHER_CTX_block_size(e)   ((e)->cipher->block_size)
 #define EVP_CIPHER_CTX_key_length(e)   ((e)->key_len)
 #define EVP_CIPHER_CTX_iv_length(e)    ((e)->cipher->iv_len)
 #define EVP_CIPHER_CTX_get_app_data(e) ((e)->app_data)
 #define EVP_CIPHER_CTX_set_app_data(e,d) ((e)->app_data=(char *)(d))
 #define EVP_CIPHER_CTX_type(c)         EVP_CIPHER_type(EVP_CIPHER_CTX_cipher(c))
 #define EVP_CIPHER_CTX_flags(e)                ((e)->cipher->flags)
 #define EVP_CIPHER_CTX_mode(e)         ((e)->cipher->flags & EVP_CIPH_MODE)

 int EVP_CIPHER_param_to_asn1(EVP_CIPHER_CTX *c, ASN1_TYPE *type);
 int EVP_CIPHER_asn1_to_param(EVP_CIPHER_CTX *c, ASN1_TYPE *type);
