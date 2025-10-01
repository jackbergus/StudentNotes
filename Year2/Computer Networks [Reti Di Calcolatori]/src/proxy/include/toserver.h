#ifndef __TOSERVER_H__
#define __TOSERVER_H__


/*	Proxy's Library	*/
#include	"communication.h"
#include	"thread_functs.h"
#include	"libhashtable.h"
#include	"init_globals.h"
#include	"toclient.h"
#include	"connect.h"
#include	"params.h"
#include	"types.h"
#include	"macro.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/** fetch():
 *  Questa funzione prende una risorsa remota e, in caso di successo,
 *  effettua il salvataggio in cache
 *  @param buffer:	Area di memoria dove verrà salvata la risorsa
 *  @param remote_path:	Percorso remoto associato alla risorsa
 *  @param fdclient:	FileDescriptor lato client: se -1, non invia nulla
 *  @return		Restituisce 1 in caso di errore, altrimenti 0
 */
int		fetch( char* buffer, char* remote_path, int fdclient);


/** handle_serverPool():
 *  Gestione delle richieste verso il server, direttamente dal client
 */
void	handle_serverPool( int whoami, msg* params);


void	handle_prefetchPool( int whoami, struct reslist* prefetch_res);


#endif
