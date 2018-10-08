#ifndef MW_ENUM_H
#define MW_ENUM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>
#include <dirent.h> /* posix folder standard */
#include <direct.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "serial/cstdinfo.h"

typedef struct{
	size_t items; /* amount of items in (uintptr_t) name array (initialized) */
	uintptr_t* name; /* name array */
	size_t name_allocated; /* how big the (uintptr_t)name array is, so a multiple of 1000 (2000,4000,8000,...) */
	size_t allocated_for_names; /* size in bytes of all heap allocated name buffers ( list->name[ 0 <--> items ] ) */
}FileList;

FileList* file_list_init(void);

void file_list_cleanup(FileList* list);

void get_file_list(byte* path,FileList* list);

void get_folder_list(byte* path, FileList* list);

void rec_get_file_list(byte* path, FileList* list);

void rec_get_folder_list(byte* path, FileList* list);

#endif
