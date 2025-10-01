#ifndef __TOCLIENT_H__
#define __TOCLIENT_H__


/*	Proxy's Library	*/
#include	"libhashtable.h"	/*	Hashtable library	*/
#include	"init_globals.h"
#include	"types.h"
#include 	"connect.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

struct request_cache
{
	int		fd_client;
	int		fd_file;
	char*	elemen;
	char  	remote[MAX_FILE];
};


void	toclient(int whoami, void* arg);

#endif
