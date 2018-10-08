#include "enum.h"

FileList* file_list_init(void)
{
	FileList* list = malloc(sizeof(FileList));
	memset(list,0x0,sizeof(FileList));
	
	list->name = calloc(1000,sizeof(uintptr_t));
	list->name_allocated = 1000;
	list->allocated_for_names = 0;
	
	return list;
}

int is_file(byte* path)
{	
	struct stat s;
	if( stat(path,&s) == 0)
	{
		if( s.st_mode & S_IFDIR )
		{
			return 0;
		}
		else if( s.st_mode & S_IFREG )
		{
			return 1;
		}
		else
		{
			cerror("is_file() different type");
			return -1;
		}
	}
	else{
		cerror("is_file() error occurred (%d)",errno);
		return -1;
	}
}

void file_list_cleanup(FileList* list)
{
	if(!list)return;
	
	for(int i = 0;i < list->items;i++)
	{
		free( (void*)(list->name[i]) );
	}
	free(list);
}

void add_to_file_list(byte* path, struct dirent* ent, FileList* list,int check_file_mode) /* check_file_mode is the return of is_file, so you can sort for dir or files */
{
	if(!path || !ent || !list)return;
	
	byte* name = ent->d_name;
	size_t path_len = strlen(path);
	size_t name_len = strlen(name);
	
	if(check_file_mode == 0 && ( !strcmp("..",name) || !strcmp(".",name) ) )
	{
		return;
	}
	size_t allocation_size = path_len + name_len + 1;
	byte* concat_block = calloc(1,allocation_size);
	memcpy(concat_block,path,path_len);
	memcpy(concat_block+path_len,name,name_len);
	concat_block[path_len+name_len] = 0x0;
	
	
	if(is_file(concat_block) != check_file_mode)
	{
		free(concat_block);
		return;
	}
	
	/* if list of files has reached its limit, allocate a new buffer 2x the old size and copy buffer over */
	if( list->items >= list->name_allocated )
	{
		size_t name_new_allocated = list->name_allocated * 2;
		#ifdef DEBUG
		cdebug("DEBUG: expanding name buffer to size %d",name_new_allocated);
		#endif
		uintptr_t* name_new = calloc(name_new_allocated,sizeof(uintptr_t));
		memcpy( name_new,list->name, list->name_allocated * sizeof(uintptr_t) );
		free(list->name);
		list->name = name_new;
		list->name_allocated = name_new_allocated;
	}

	/* append concatenated block to end of name list */
	list->name[list->items] = (uintptr_t)concat_block;
	list->items++;
	list->allocated_for_names+= allocation_size;
	
}

void get_file_list(byte* path,FileList* list)
{
	if(!path)return;
	DIR* dir;
	struct dirent * ent;
	if( (dir = opendir(path)) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
				add_to_file_list(path,ent,list,1);
		}
		closedir(dir);
	}
	else
	{
		cerror("cannot open dir %s",path);
	}
	
	return;
	
}

void get_folder_list(byte* path, FileList* list)
{
	if(!path)return;
	DIR* dir;
	struct dirent * ent;
	if( (dir = opendir(path)) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
				add_to_file_list(path,ent,list,0);
		}
		closedir(dir);
	}
	else
	{
		cdebug("cannot open dir %s",path);
	}
	
	return;
}

void rec_get_file_list(byte* path, FileList* list)
{
	DIR* dir;
	struct dirent* ent;
	int count = 0;
	if( (dir = opendir(path)) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
				count++;
				byte* name = ent->d_name;
				size_t path_len = strlen(path);
				size_t name_len = strlen(name);
			
				size_t allocation_size = path_len + name_len + 2;
				byte* concat_block = calloc(1,allocation_size);
				memcpy(concat_block,path,path_len);
				concat_block[path_len] = '\\';
				memcpy(concat_block+path_len+1,name,name_len);
				concat_block[path_len+name_len+1] = 0x0;
				
				cdebug("loop it %d path %s",count,concat_block);
				if(is_file(concat_block) != 1)
				{
					cdebug("entered not file loop");
					if( !strcmp(".",name) || !strcmp("..",name) )
					{
							
					}
					else if(is_file(concat_block) == 0)
					{
						cdebug("getting rec info for path %s",concat_block);
						rec_get_file_list(concat_block,list);
					}
					free(concat_block);
				}
				else
				{
					/* if list of files has reached its limit, allocate a new buffer 2x the old size and copy buffer over */
					if( list->items >= list->name_allocated )
					{
						size_t name_new_allocated = list->name_allocated * 2;
						#ifdef DEBUG
						cdebug("DEBUG: expanding name buffer to size %d",name_new_allocated);
						#endif
						uintptr_t* name_new = calloc(name_new_allocated,sizeof(uintptr_t));
						memcpy( name_new,list->name, list->name_allocated * sizeof(uintptr_t) );
						free(list->name);
						list->name = name_new;
						list->name_allocated = name_new_allocated;
					}

					/* append concatenated block to end of name list */
					//cdebug("file at path %s",concat_block);
					list->name[list->items] = (uintptr_t)concat_block;
					list->items++;
					list->allocated_for_names += allocation_size;
				}
		}
		closedir(dir);
	}
	else
	{
		cdebug("cannot open dir %s",path);
	}
	return;
}



void rec_get_folder_list(byte* path, FileList* list)
{
	DIR* dir;
	struct dirent* ent;
	int count = 0;
	if( (dir = opendir(path)) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
				count++;
				byte* name = ent->d_name;
				size_t path_len = strlen(path);
				size_t name_len = strlen(name);
			
				size_t allocation_size = path_len + name_len + 2;
				byte* concat_block = calloc(1,allocation_size);
				memcpy(concat_block,path,path_len);
				concat_block[path_len] = '\\';
				memcpy(concat_block+path_len+1,name,name_len);
				concat_block[path_len+name_len+1] = 0x0;
				
				cdebug("loop it %d path %s",count,concat_block);
				if(is_file(concat_block) != 0)
				{
					free(concat_block);
				}
				else if( !strcmp(".",name) || !strcmp("..",name) )
				{
					free(concat_block);
				}
				else
				{
					cdebug("entered else loop to save path");
					/* if list of files has reached its limit, allocate a new buffer 2x the old size and copy buffer over */
					if( list->items >= list->name_allocated )
					{
						size_t name_new_allocated = list->name_allocated * 2;
						#ifdef DEBUG
						cdebug("DEBUG: expanding name buffer to size %d",name_new_allocated);
						#endif
						uintptr_t* name_new = calloc(name_new_allocated,sizeof(uintptr_t));
						memcpy( name_new,list->name, list->name_allocated * sizeof(uintptr_t) );
						free(list->name);
						list->name = name_new;
						list->name_allocated = name_new_allocated;
					}

					/* append concatenated block to end of name list */
					//cdebug("file at path %s",concat_block);
					list->name[list->items] = (uintptr_t)concat_block;
					list->items++;
					list->allocated_for_names += allocation_size;
					
					rec_get_folder_list(concat_block,list);
				}
		}
		closedir(dir);
	}
	else
	{
		cdebug("cannot open dir %s",path);
	}
	return;
}



/* example function */

/*
int main(void)
{	
	FileList* list = file_list_init();
	
	byte path[] = "C:\\Users\\user\\Desktop\\test_folder";
	
	rec_get_file_list(path,list);
	
	for(int i = 0;i < list->items;i++)
	{
			printf("%s\n",list->name[i]);
	}
	printf("total files: %d - allocated %d bytes\n",list->items,list->allocated_for_names);
	file_list_cleanup(list);
	

	
	return 0;
}
*/