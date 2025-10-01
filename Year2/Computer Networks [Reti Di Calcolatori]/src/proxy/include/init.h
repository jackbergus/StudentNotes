
#ifndef		__INIT_H__
#define		__INIT_H__


/*	Project's Library	*/
#include	"communication.h"
#include	"thread_functs.h"
#include	"libhashtable.h"	/*	Hashtable library	*/
#include	"toserver.h"
#include	"toclient.h"
#include	"connect.h"
#include	"params.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

 
/** start_threads():
 *  Funzione di inizializzazione dei threads e delle Pool 
 */
int		start_threads();


/** init_proxy_clientside():
 *  Predispone il proxy ad accettare richieste lato client 
 */
int		init_proxy_clientside(struct proxy *data);


/** handle_client_request():
 *  Accetta una richiesta dal client 
 *  @param data:	Informazione di stato del proxy
 *  @param fd_client:	Se la procedura termina con successo, setta il fd del client 
 */
request* handle_client_request(struct proxy *data, int *fd_client);


#endif
