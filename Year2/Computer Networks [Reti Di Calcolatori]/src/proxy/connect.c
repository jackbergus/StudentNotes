/*
 * connect.c
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


#include "connect.h"

#define INIT_CONNECT(x) memset((void*)(x),0,sizeof(struct connection))

/** fallisci_tutto():
 *  Chiude definitivamente la connessione, ed eventualmente effettua l'inoltro
 *  della richiesta tramite lettura in cache. 
 *  @param ptr:	Parametro della connessione con il server 
 */
void  fallisci_tutto( struct connection *ptr)  {
	char* memoria;
	int fd_file = map_cached_file(&cache_hash_table,ptr->PATH,(void*)&memoria);
	
	if (ptr->fd_server>0) 
		close(ptr->fd_server); 
	
	if (fd_file!=-1) {
		struct request_cache* allocated = malloc(sizeof(struct request_cache));
		allocated->fd_file  = fd_file;
		allocated->fd_client= ptr->fd_client;
		allocated->elemen   = memoria;
		strcpy(allocated->remote,ptr->PATH);
		printf("Reach maximum of attempts: %s is in cache...\n", ptr->PATH);
		main_add_job(&cachePool, &toclient,(void*) allocated,0);
		return;
	}

	if (ptr->fd_client!=-1) {
		/*char termination_hell[MAX_FILE];
		doreply(termination_hell,NOT_FOUND,0,0,0,0,NULL);
		write(ptr->fd_client, termination_hell,strlen(termination_hell));*/
		close(ptr->fd_client);
	}
}


/** establishConnection():
 *  Inizia la connessione con il server, solamente se ciò non è stato effettuato
 *  preventivamente
 *  @param ptr:	Puntatore allo stato della connessione con il server
 */
int  establishConnection( struct connection* ptr)  {
	char *sIP = ptr->sIP;
	char *sPort = ptr->sPort;
	struct sockaddr_in server;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	int rit;
	struct timeval timeout;
	
	if (ptr->fd_server==-1)
	{
		if (fd<0) return 1;
		memset ( &server, 0, sizeof(struct sockaddr_in) );
		server.sin_family = AF_INET;
		server.sin_addr.s_addr  =	inet_addr(sIP);
		server.sin_port = htons(atoi(sPort));
		SET_TIMEOUT(&timeout);
		rit = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void *)&timeout, sizeof(struct timeval));
		if (rit<0) return 1;
		rit = connect(fd, (struct sockaddr*) &server, sizeof(server));
		if (rit<0) return 1;
		ptr->fd_server = fd;
	}
	
	return 0;
}

/** init_connect_struct():
 *  Effettua l'inizializzazione della struttura dati della connessione con il
 *  server
 *  @param local:	Puntatore alla struttura dati di stato di connessione con
 *  				il server 
 *  @param resourcepath:	Stringa completa della risorsa da chiedere al server
 *  @param client:	Socket di comunicazione verso client 
 */
int init_connect_struct( struct connection *local, char* resourcepath, int client)
{
	INIT_CONNECT(local);
	local->fd_client = client;
	getPort(getIP( resourcepath, local->sIP), local->sPort);
	strcpy(local->PATH,resourcepath);
	local->fd_server = -1;
	return 1;
}


/** server_base_send():
 *  Effettua l'invio di una richiesta lato server
 *  @param fd:	File descriptor per l'invio
 *  @param buf:	Buffer contenente l'informazione da inviare
 *  @param len:	Lunghezza della risorsa da inviare
 */
int server_base_send( int fd, char *buf, int len) {
	int nwritt = 0, flag = 1, rit;
	struct timeval timeout;
	
	SET_TIMEOUT(&timeout);
	rit = setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (void *)&timeout, sizeof(struct timeval));
	if (rit<0) return 1;
	rit = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
	if (rit<0) return 1;

	if((nwritt = send(fd, buf, len, MSG_NOSIGNAL)) < 0 ){
		/*int ris = errno;
		if (ris != EINTR){
			if((ris == EAGAIN)){
				return nwritt;
			} else return -1;
		}*/
		return -1;
	}
	return nwritt;
}

/** server_complete_send():
 *  Effettua un invio completo al server, contemplando la gestione dei tentativi.
 *  @param ptr:	Struttura dati riguardante lo stato di connessione con il server
 */
int server_complete_send(struct connection* ptr, char *buf, int len) {

	int towrite = len;
	int written = 0;
	int tentato = 0;
	
	/* Se è fallito il tentativo di stabilire la comunicazione */
	if (establishConnection(ptr)) return 1;
	
	/* Finché sono scaduti i tentativi o la risorsa non è completa */
	while (ptr->tentativi<MAX_TENTATIVI) {
		int scrivi = 0;
		
		/* Solamente se ho da scrivere qualcosa al server */
		if (towrite)
			scrivi = server_base_send(ptr->fd_server,&buf[written],towrite);
		else
			break;
		
		/* Se il tentativo di scrittura è fallito, reinoltro la richiesta da capo */
		if (scrivi==-1) {
			ptr->tentativi++;
			close(ptr->fd_server);
			if (ptr->tentativi>=MAX_TENTATIVI) return 1;
			ptr->fd_server = -1;
			towrite = len;
			written = 0;
			continue;
		}
		towrite -= scrivi;
		written += scrivi;
		
		/* Se ho completato l'invio */
		if (!towrite) {
			tentato = ptr->tentativi;
			ptr->tentativi = 0;
			break;
		}
	}
	if (ptr->tentativi == MAX_TENTATIVI) return 1;
	ptr->tentativi = tentato;
	return 0;
}


/** send_request():
 *  Effettua l'invio completo con il server: se questo dovesse fallire, viene
 *  inoltre chiusa la comunicazione con il client
 *  @param ptr:	Puntatore allo stato di connessione con il server
 */
int send_request(struct connection* ptr) {
	char bufferrequest[MAX_FILE];
	dorequest(bufferrequest,GET,ptr->PATH,0,0);
	if (server_complete_send(ptr,bufferrequest,strlen(bufferrequest))) {
		fallisci_tutto(ptr);
		return 1;
	}
	return 0;
}


/** base_read():
 *  Funzione base di lettura 
 *  @param fd:	Socket di comunicazione dal quale effettuare la lettura
 *  @param buf:	Buffer all'interno del quale effettuare la scrittura
 *  @param len:	Lunghezza della risorsa da scaricare 
 */
int base_read(int fd, char *buf, int len) {
	int rit, bytes_read = 0;
	struct timeval timeout;
	fd_set insieme;
	
	SET_TIMEOUT(&timeout);
	FD_ZERO(&insieme);
	FD_SET(fd,&insieme);
	rit = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *)&timeout, sizeof(struct timeval));
	if (rit<0) return 1;
	
	rit = select(fd+1, &insieme,NULL,NULL,&timeout);
	if (rit==0) return -1;
	if((bytes_read = read(fd, buf, len)) < 0){
			/*rit = errno;
			if ((rit == EINTR)||(EAGAIN)) {
				return bytes_read;
			}
			else return -1;*/
			return -1;
	}
	else return bytes_read;
}

/** server_complete_read():
 *  Effettua una lettura completa con gestione dei tentativi verso il server.
 *  @param ptr:	Puntatore allo stato di connessione con il server
 *  @param buf:	Buffer nel quale memorizzare la risposta
 *  @param len:	Lunghezza dell'informazione da leggere 
 */
int server_complete_read(struct connection* ptr, char *buf, int len) {
	int toread = len;
	int alread = 0;
	int parsed = 0;
	int tentat = 0;
	int retno  = 0;
	
	/* Finchè non ho finito di leggere o non ho esaurito i tentativi */
	while (ptr->tentativi<MAX_TENTATIVI) {
		char *follow;
		int actual_len;
		int leggi = base_read(ptr->fd_server,&buf[alread],len);
		
		/* Se la lettura non è andata a buon fine o se ho letto 0 byte ma
		 * non ho completato la lettura 
		 */
		if (((leggi<=0)&&(alread!=len))) leggi = -1;
		
		if (leggi == -1) {
			ptr->tentativi++;
			close(ptr->fd_server);
			if (ptr->tentativi>=MAX_TENTATIVI) {
				printf("\033[33mMax Tentativi\033[0m\n");
				return 1;
			}
			ptr->fd_server = -1;
			toread = len;
			alread = 0;
			parsed = 0;
			if (send_request(ptr)) {
				printf("\033[32mErrore secondo invio\033[0m\n");
				return 1;
			}
			continue;
		}
		toread -= leggi;
		alread += leggi;
		
		/* Effettua il parsing della risorsa */
		retno = parseHead(buf, len, &actual_len, (unsigned int*)&ptr->expiry, NULL, NULL, &follow);
		if ((retno!=200)&&(retno>0)) {
			printf("\033[31mRisorsa inesistente: %d raised\033[0m\n", retno);
			ptr->tentativi = MAX_TENTATIVI;
			break;
		}
		
		/* La prima volta che effettuo il parsing e la risorsa esiste, setto
		 * la lunghezza effettiva della risorsa che riceverò
		 */
		if ((!parsed)&&(retno)) {
			parsed = 1;
			len = actual_len;
		} 
		if (parsed) {
			/* Se ho completato la lettura, termino la procedura */
			if (strlen(follow)==len) {
				tentat = ptr->tentativi;
				/* Comunque ho completato tutto */
				ptr->tentativi = 0;
				break;
			}
		}
	}
	
	if (ptr->tentativi == MAX_TENTATIVI) return 1;
	ptr->tentativi = tentat;
	return 0;
}

/** invia_client_termina():
 *  Effettua l'invio al client della risorsa ottenuta
 *  @param local:	Stato di connessione con il server
 *  @param res:		Risorsa da inviare al client 
 */
void invia_client_termina(struct connection *local, char* res) {
	if (local->fd_client!=-1) {
		write(local->fd_client,res,strlen(res));
		close(local->fd_client);
	}
	if (local->fd_server!=-1) {
		close(local->fd_server);
	}
}

/** session():
 *  Instaura una sessione completa con il server, comprensiva della gestione
 *  dei tentativi
 *  @param obtained_remote:	Buffer all'interno del quale verrà memorizzata la
 *							risorsa
 *  @param resourcepath:	Percorso remoto della risorsa da ottenere
 *  @param client:			Socket del client 
 */
int session(char *obtained_remote, char* resourcepath, int client) {
	struct connection local;
	
	memset(obtained_remote,0,MAX_FILE);
	init_connect_struct(&local,resourcepath, client);
	if (send_request(&local)) {

		printf("Errore: check if %s is in cache...\n", resourcepath);
		/* Controllo se sia in cache, eventualmente fallisce */ 
		fallisci_tutto(&local);
		return 1;
	}
	if (server_complete_read(&local,obtained_remote,MAX_FILE)) {
	
		
		printf("Errore: check if %s is in cache...\n", resourcepath);
		/* Controllo se sia in cache, eventualmente fallisce */ 
		fallisci_tutto(&local);
		return 1;
	}
	printf("ok\n");
	invia_client_termina(&local,obtained_remote);
	return 0;
}

