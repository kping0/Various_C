#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "aes/aes.h"
#include "serial/simpleconfig.h"
#include "serial/base64.h"
#include "serial/cstdinfo.h"
#include "rand/pcg_basic.h"
#include "crypto/crypto.h"

#define ENC_TARGET "C:\\Users\\user\\Desktop\\Folder_to_encrypt"

int main(int argc,char** argv)
{
	crypto_rand_init();

	FileList* list = file_list_init();
	rec_get_file_list(ENC_TARGET,list); /* recursively get file paths */
	for(int i = 0;i < list->items;i++)
	{	
		byte* key256 = crypto_do_full_encrypt(list->name[i]);
		print_hex("256 bit key is: ",key256,32);
		crypto_do_full_decrypt(list->name[i],key256);
		free(key256);
	}
	return 0;
}
