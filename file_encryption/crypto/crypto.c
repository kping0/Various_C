/* crypto.c - general functions to encrypt/decrypt files */
#include "crypto.h"

void clear_file(byte* path)
{
	if(!path)return;
	FILE* tmp = fopen(path,"w");
	if(tmp)fclose(tmp);
}

void write_file(byte* path, byte* buf, size_t len)
{
	if(!path || !buf || !len)return;
	
	FILE* wp = fopen(path,"wb");
	if(wp)
	{
		fwrite(buf,len,1,wp);
		fclose(wp);
	}
}

void crypto_rand_init(void)
{
	int rand_loc = 10; /* definition is not important, as only address of int is used in rand init */	
	pcg32_srandom(time(NULL) ^ (intptr_t)&printf, (intptr_t)&rand_loc);
}

void get_rand_buf(void* buf_v, size_t size)
{
	byte* buf = buf_v;
	int i;
	for(i = 0;i < size; i++)
	{
		uint32_t rand_num = pcg32_random(); /* should be seeded first */
		buf[i] = *((byte*)(&rand_num)); /* copy first byte of uint32_t to buf */
	}
}

void print_hex(byte* prefix, byte* tmp, size_t size)
{
	printf("%s",prefix);
	for(int i = 0;i < size;i++)
	{
		printf("0x%x, ",tmp[i]);
	}
	printf("\n\n");
}

EncFile* enc_file_init(byte* path)
{
	if(!path)return NULL;
	
	EncFile* enc_file = malloc(sizeof(EncFile));
	get_rand_buf(&(enc_file->key),32);
	get_rand_buf(&(enc_file->iv),16);
	#ifdef DEBUG
	print_hex("key",enc_file->key,32);
	print_hex("iv",enc_file->iv,16);
	#endif
	
	size_t path_len = strlen(path);
	
	/* move path to heap */
	byte* path_tmp = malloc( path_len + 1 );
	memcpy(path_tmp,path,path_len);
	path_tmp[path_len] = 0x0;
	enc_file->path = path_tmp;
	
	/* open file and get file length */
	FILE* fp = fopen(path,"rb");
	if(!fp)
	{
		free(path_tmp);
		free(enc_file);
		return NULL;
	}
	fseek(fp,0,SEEK_END);
	enc_file->tmp_len = ftell(fp);
	fseek(fp,0,SEEK_SET);
	
	/* read entire file contents to tmp*/
	enc_file->tmp = malloc(enc_file->tmp_len + 1);
	fread(enc_file->tmp,enc_file->tmp_len,1,fp);
	fclose(fp);
	#ifdef DEBUG
	print_hex("input data: ",enc_file->tmp,enc_file->tmp_len);
	#endif
	return enc_file;
}

void enc_file_cleanup(EncFile* encobj)
{
	if(!encobj)return;
	
	free(encobj->tmp);
	free(encobj->path);
	free(encobj);
}
 
void enc_file_write(EncFile* encobj) /* write buffer contents to a file */
{
	if(!encobj)return;
	
	clear_file(encobj->path);
	
	SCONFIG* tmp = sconfig_load(encobj->path);
	sconfig_set(tmp,"aes",encobj->tmp,encobj->tmp_len);
	sconfig_set(tmp,"iv",encobj->iv,16);
	sconfig_write(tmp);
	sconfig_close(tmp);
	
	return;
}

void enc_file_xcrypt(EncFile* encobj)
{
	if(!encobj)return;
	
	AES_init_ctx_iv(&(encobj->ctx), encobj->key, encobj->iv); /* set ctx to key in struct */
	
	AES_CTR_xcrypt_buffer(&(encobj->ctx), encobj->tmp, encobj->tmp_len);
	#ifdef DEBUG
	print_hex("xcrypted data is: ",encobj->tmp,encobj->tmp_len);
	#endif
	return;
}

void crypto_do_full_decrypt(byte* enc_path, byte* key)
{
	if(!enc_path || !key)return;
	
	EncFile* encobj = malloc(sizeof(EncFile));

	/* copy over key */
	memcpy(encobj->key,key,32);
	
	/* get aes data & iv */
	SCONFIG* tmp = sconfig_load(enc_path);
	if(!tmp)return;
	
	sscsd* enc_data = sconfig_get_full(tmp,"aes");
	if(!enc_data)return;
	encobj->tmp = enc_data->data;
	encobj->tmp_len = enc_data->len;
	
	sscsd* iv_data = sconfig_get_full(tmp,"iv");
	if(!iv_data)return;
	memcpy(encobj->iv,iv_data->data,16);
	
	SSCS_data_release(&iv_data);
	sconfig_close(tmp);
	/******************/
	
	/* decrypt data */
	enc_file_xcrypt(encobj);
	#ifdef DEBUG
	print_hex("xcrypted data is: ",encobj->tmp,encobj->tmp_len);
	#endif
	/* cleanup and write decrypted data back to file */
	clear_file(enc_path);
	write_file(enc_path,encobj->tmp,encobj->tmp_len);
	
	free(encobj);
	SSCS_data_release(&enc_data);
	return;
}

byte* crypto_do_full_encrypt(byte* enc_path) /* encrypt file and return 32 byte key */
{
	EncFile* encobj = enc_file_init(enc_path);
	enc_file_xcrypt(encobj);
	enc_file_write(encobj);
	
	byte* key256 = malloc(32); /* 32 * 8bit = 256bit */
	memcpy(key256,encobj->key,32);
	
	enc_file_cleanup(encobj);
	
	return key256;
}