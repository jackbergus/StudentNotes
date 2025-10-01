#ifndef __CONNECT__H__
#define __CONNECT__H__

#include <netinet/tcp.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "init_globals.h"
#include "consts.h"
#include "macro.h"
#include "types.h"
#include "communication.h"
#include "thread_functs.h"

#define MAX_TENTATIVI 10

void invia_client_termina(struct connection *local, char* res);

void  fallisci_tutto( struct connection *ptr);

/** session():
 *  Effettua la connessione con un server, fornendo la risposta
 *  ad un dato client.
 *  @param obtained_remote:	Buffer di memorizzazione della risposta
 *  @param resourcepath:	Percorso della risorsa remota
 *  @param client:		FileDescriptor del client
 *  @return			Restituisce 1 in caso di fallimento, altrimenti 0
 */
int session(char* obtained_remote, char* resourcepath, int client);

#endif
