/*
 * toserver.c
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
#include	"toserver.h"

/** fetch():
 *  Richiesta della risorsa ed eventuale memorizzazione nel filesystem */
int  fetch(char* buffer, char* remote_path, int fdclient) 
{
	/* Solamente se la comunicazione client-server è andata a buon fine */
	if (!session(buffer, remote_path,fdclient)) 
	{
		/* possiamo effettuare il salvataggio su file */
		handle_file_filesystem(&cache_hash_table,remote_path, (void*)buffer, 1);
		return 0;
	}
	return 1;
}

/** handle_serverPool():
 *  Gestione della richiesta client 
 *  @param whoami:	Indicatore del thread corrente
 *  @param params:	Parametri indicanti la richiesta
 */
void handle_serverPool( int whoami, msg* params)
{
	 
	char obtained_remote[MAX_FILE]; 
	char *cont;
	char *memoria;
	
	int fd_file = map_cached_file( &cache_hash_table, params->rq.PATH, 
													  (void*) &memoria);
	/*	il file è presente in cache	*/
	if( fd_file!=-1) 
	{
		struct request_cache *allocated = malloc( sizeof( struct request_cache));
		allocated->elemen				= memoria;
		allocated->fd_file				= fd_file;
		allocated->fd_client			= params->sock;
		strcpy( allocated->remote, params->rq.PATH);
		main_add_job( &cachePool, &toclient,(void*) allocated, 0);
		free( params);
		return;
	}
		
	printf("\033[35mWhoami: %d Handling: %s\033[0m\n", whoami, params->rq.PATH);
	
	/* Gestione della connessione con il server */
	if( fetch( obtained_remote, params->rq.PATH, params->sock)) 
	{
		free( params);	
		return;
	}

	printf("\033[35mPREFETCHING with lvl=%d\033[0m\n",global.level);
	
	/* Solamente se il parsing della risorsa è corretta */
	if( parseHead( obtained_remote, strlen(obtained_remote), NULL, NULL, NULL,NULL,&cont))
	{
		update_list(cont, handle_prefetchPool, 1, global.level, 0, strlen(obtained_remote));
	}
	

}


/** handle_prefetchPool():
 *  Gestione del prefetching
 *  @param whoami:		Indicatore del thread corrente
 *  @param prefetch_res:Indicazione della risorsa da ottenere 
 */
void handle_prefetchPool( int whoami, struct reslist* prefetch_res)
{  
	char obtained_remote[MAX_FILE]; 
	char *body;
	char *memoria;
	int fd_file;
	
	fd_file = map_cached_file( &cache_hash_table, prefetch_res->remote_path, 
												  (void*)&memoria);
	/*	il file è in cache	*/
	if( fd_file!=-1)
	{
		printf("\033[31mThe file %s is already in cache...\n", prefetch_res->remote_path);
		close_cached_file( &cache_hash_table, prefetch_res->remote_path, 
											  (void*) memoria,fd_file);
		free( prefetch_res->remote_path);
		free( prefetch_res);
		return;
	}
	
	printf("\033[31mPrefetching : %s\033[0m\n", prefetch_res->remote_path);
	/*	ottengo la risorsa	*/
	if( fetch( obtained_remote, prefetch_res->remote_path, -1))
	{
		free( prefetch_res->remote_path);
		free( prefetch_res);
		return;
	}
	
	parseHead( obtained_remote, strlen( obtained_remote), NULL, NULL, NULL,
																	  NULL,
																	  &body);
	
	/*	Controllo se ci sono risorse di cui fare il prefetch	*/
	update_list( body, handle_prefetchPool, prefetch_res->prefetch_num,
											global.level,
											0,
											strlen( body));
	free( prefetch_res->remote_path);
	free( prefetch_res);
	
}

