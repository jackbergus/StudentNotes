/*
 * fsys.c
 * This file is part of hashtable
 *
 * Copyright (C) 2012 - Giacomo Bergami
 *
 * hashtable is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * hashtable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hashtable; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


#include	"fsys.h"


int FileCreate(char* remote_resource, void* file) {
	char *local;
	int fd;
	int ret;
	
	local = obtain_local(remote_resource);
	re_new_resource(local,NULL,&fd);
	if (fd<0) {
		printf("Error raised: %s\n", local);
		free(local);
		return 0;
	}
	/* XXX: */
	ret = write(fd,(void*)file,strlen((char*)file));
	if (ret<0) {
		close(fd);
		free(local);
		return 0;
	}
	close(fd);
	free(local);
	return 1;
}


int FileOpen(char* remote_resource,void** update) {
	char *local = obtain_local(remote_resource);
	int fd = open(local,O_RDONLY);
	int ret;
	if (fd<0) {
		free(local);
		return -1;
	}
	if (!((update))) return 0; 
	/**update = mmap(0,MAX_FILE,PROT_READ,MAP_SHARED,fd,0);*/
	*update = malloc(MAX_FILE);
	if (*update==NULL) ret = -1;
	if (ret<0) {
		close(fd);
		free(local);
		free(*update);
		return -1;
	}
	memset(*update,0,MAX_FILE);
	if ((read(fd,*update,MAX_FILE))==-1) {
		close(fd);
		free(local);
		free(*update);
		return -1;
	}
	close(fd);
	printf("File found!!! \033[34m%s\033[0m\n", local);
	free(local);
	return fd;
}


int FileClose(void* update, int fd) {
	/*int val = munmap(update,MAX_FILE);
	if (fd!=-1) close(fd);
	return (val!=-1); */
	free(update);
	return	(1);
}
