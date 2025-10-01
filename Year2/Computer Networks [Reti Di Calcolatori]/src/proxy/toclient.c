/*
 * toclient.c
 * This file is part of proxy
 *
 * Copyright (C) 2012 - Giacomo Bergami, Paolo de Luca, Elena Fabbri
 *
 * proxy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * proxy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with proxy; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


#include	"toclient.h"

/** toclient():
 *  Inoltra la risposta al client, tramite lettura della cache 
 */
void toclient( int whoami, void* arg) 
{
	struct connection		local;	
	struct request_cache* 	ptr;
	
	char	*remote, *memoria;
	int 	fd_file;
	
	ptr 		= (struct request_cache*) arg;
	fd_file 	= ptr->fd_file;
	memoria 	= ptr->elemen;
	remote		= ptr->remote;
	
	printf("\033[33mClient (handling direct cache)...\033[0m\n");

	local.fd_server = -1;
	local.fd_client = ptr->fd_client;
	
	invia_client_termina( &local, memoria);

	/* Liberazione della memoria associata alla risorsa */	
	close_cached_file( &cache_hash_table, remote, (void*)memoria, fd_file);
	free(arg);
}
