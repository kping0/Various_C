#ifndef MW_CRYPTO_H
#define MW_CRYPTO_H

#include "aes/aes.h"
#include "serial/simpleconfig.h"
#include "serial/base64.h"
#include "serial/cstdinfo.h"
#include "rand/pcg_basic.h"

typedef struct{
	
	byte key[32]; /* 256 bit key */
	byte iv[16]; /* 128 bit iv */
	
	struct AES_ctx ctx;
	
	byte* tmp;
	size_t tmp_len;
	
	byte* path;
}EncFile;

void clear_file(byte* path);

void write_file(byte* path, byte* buf, size_t len);

void crypto_rand_init(void);

void get_rand_buf(void* buf_v, size_t size);

void print_hex(byte* prefix, byte* tmp, size_t size);

EncFile* enc_file_init(byte* path);

void enc_file_cleanup(EncFile* encobj);
 
void enc_file_write(EncFile* encobj); /* write buffer contents to a file */

void enc_file_xcrypt(EncFile* encobj);

void crypto_do_full_decrypt(byte* enc_path, byte* key_iv_path);

byte* crypto_do_full_encrypt(byte* enc_path);

#endif