/*
 * init.c
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
 * along with proxy. If not, see <http://www.gnu.org/licenses/>.
 */
#include	"init.h"



			/**********************************/
			/* Variabili riguardanti i thread */
			/**********************************/
			
pthread_attr_t 		pt_attr;
pthread_t		server[4];
pthread_t		prefetch;
pthread_t		cache;

SCHEDULING		serverPool;
SCHEDULING		prefetchPool;
SCHEDULING		cachePool;


			/********************************/
			/* Variabile di stato del Proxy */
			/********************************/
struct 	proxy 		global;
ARGS_ONE 		prefetch_arg;
ARGS_ONE		cache_arg;
ARGS_ONE		server_thread_args[4];


			/************************/
			/* Gestione della Cache */
			/************************/
HRES 			cache_hash_table;



int start_threads() 
{
	int i;
	
	/* Inizializzazione delle strutture dati */
	ONCONDRET1(pthread_attr_init(&pt_attr));
	ONCONDRET1(pthread_attr_setdetachstate(&pt_attr, PTHREAD_CREATE_DETACHED));
	ONCONDRET1(init_scheduler(&serverPool));
	ONCONDRET1(init_scheduler(&prefetchPool));
	init_hres(&cache_hash_table);
	
	/* Creazione dei thread(s) */
	for (i=0; i<4; i++) 
	{
		server_thread_args[i].whoami = i;
		server_thread_args[i].sched  = &serverPool;
		ONCONDRET1(pthread_create (&server[i], &pt_attr, thread_memento, (void*) &server_thread_args[i] ));	
	}
	prefetch_arg.whoami = 4;
	prefetch_arg.sched = &prefetchPool;
	ONCONDRET1(pthread_create (&prefetch, &pt_attr, thread_memento, (void*) &prefetch_arg));
	
	cache_arg.whoami = 5;
	cache_arg.sched = &cachePool;
	ONCONDRET1(pthread_create (&cache, &pt_attr, thread_memento, (void*) &cache_arg));
	return 0;
}


/** init_proxy_clientside():
 *  Effettua l'inizializzazione della connessione, in modo da essere in grado di
 *  accettare le richieste
 *  @param data: Puntatore allo stato di connessione con il client
 */
int  init_proxy_clientside(struct proxy *data) 
{
	int optval = 1;
	data->fd = socket(AF_INET,SOCK_STREAM,0);
	ONCONDRET1(data->fd == SOCKET_ERROR);
	ONCONDRET1(setsockopt(data->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))<0);
	memset ( &data->meStesso, 0, sizeof(data->meStesso));		
	data->meStesso.sin_family	= AF_INET;
	data->meStesso.sin_addr.s_addr= data->ip;	
	data->meStesso.sin_port	= htons(data->port);
	ONCONDRET1(bind(data->fd, (struct sockaddr*) &data->meStesso, sizeof(data->meStesso))<0);	
	/* SI può bloccare ???? */
	ONCONDRET1(listen(data->fd, MAXCONN) == SOCKET_ERROR);
	return 0;
}


/* handle_client_request():
 * Effettua l'ottenimento della richiesta di un client
 * @param data:		Puntatore allo stato di connessione lato client
 * @param fd_client:	Puntatore ove verrà memorizzato il socket client
 * @return:		Se la richiesta è ben formata, restituisce un puntatore
 *			ad una richiesta allocata dinamicamente, altrimenti
 *			restituisce un puntatore a NULL
 */
request* handle_client_request(struct proxy *data, int *fd_client) 
{
	struct sockaddr_in client;
	int nread = 0, ris = 0, n = 0;
	int limit = MAXREQUEST;
	int fd;
	int len = sizeof(client);
	char buf[MAXREQUEST];
	request*	ptrRes;
	
	kassert(fdclient, "Error: NULL fdclient pointer\n");
	memset((void*)&client,0,sizeof(client));
	fd = accept(data->fd, (struct sockaddr*) &client, (socklen_t *)&len);
	if (fd == SOCKET_ERROR) return NULL;
	/* Leggo tutta l'informazione inviata dal client */
	if (((n=read(fd, &(buf[nread]), limit)) >0) ) 
	{
		nread += n;
		limit -= n;		
		ris = 1;
	}
	if (!ris) 
	{
		close(fd);
		return NULL;
	}
	ptrRes = parseRequest(buf, strlen(buf));
	/* Se la richiesta è mal formata */
	if (!ptrRes) 
	{
		doreply(buf, WRONG_REQUEST, 0, 0, 0, 0, NULL);
		do {
			n=write(fd, buf, strlen(buf));
		} while( (n<0) && (errno==EINTR));
		close(fd);
		return NULL;
	}
	*fd_client = fd;
	return ptrRes;
}



int  main(int argc, char** argv) 
{
	recursiveDelete("cache");
	/*	se c'è un errore nella raccolta di ip e porta	*/
	if( init_params( argc, argv, &global)) 
		return 1;
	/*	se c'è un errore nella creazione dei thread per le pools	*/
	if( start_threads()) 
		return 1;
	/*	se non riusciamo ad inizializzare il proxy	*/
	if( init_proxy_clientside( &global)) 
		return 1;

	/*	accepting-loop	*/
	while (1) 
	{
		int fd_client;
		int fd_file;
		char* memoria;
		
		request* prq = handle_client_request( &global, &fd_client);
		
		/*	c'è una richiesta da gestire	*/
		if( prq) 
		{
			msg* params;
			
			fd_file = map_cached_file(&cache_hash_table,prq->PATH,(void*)&memoria);
			
			/*	se il file esiste in cache	*/
			if( fd_file != -1) 
			{
				struct request_cache* allocated = malloc( sizeof(struct request_cache));
				allocated->fd_file  = fd_file;
				allocated->fd_client= fd_client;
				allocated->elemen   = memoria;
				strcpy(allocated->remote,prq->PATH);
				/* Aggiungo la richiesta di lettura in cache */
				main_add_job(&cachePool, &toclient,(void*) allocated,0);
				free(prq);
				
				continue;	/* Va ad accettare un'altra richiesta */
			}
			
			params = (msg*) malloc(sizeof(msg));
			
			params->sock	= fd_client;
			params->rq 		= *prq;
			free( prq);
			
			printf("handling: %s\n", params->rq.PATH);
			/* Aggiungo la richiesta di download da server */
			main_add_job(&serverPool, &handle_serverPool,(void*) params,0);
			
		}
		else 
		{
			printf("erroneus request\n");
			struct connection error;
			error.fd_client = fd_client;
			fallisci_tutto(&error);
		}
	}	
}


