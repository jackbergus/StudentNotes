#ifndef		__FSYS_H__
#define		__FSYS_H__


/*	Standard Library	*/
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/mman.h>
#include	<fcntl.h>


/*	Project's Library	*/
#include	"filesystem.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/** FileCreate():
 *  Questa funzione effettua la creazione del file, dato il nome
 *  della risorsa remota.
 *  @param remote_resoruce: risorsa remota
 *  @param expiry:	tempo di expire
 *  @param buffer:	contenuto dell'informazione
 *  @return			restituisce 0 in caso di insuccesso, altrimenti 1
 */
int		FileCreate(char* remote_resource, void* file);


/** FileOpen():
 * 	Questa funzione effettua l'apertura di un file che si assume non
 *  essere scaduto.
 *  @param remote_resource:	indicazione della risorsa remota
 *  @param update:	Indicazione dell'area di allocazione del file in cache
 *  @return			Restituisce 0 in caso di insuccesso, altrimenti il valore
 * 					del filedescriptor
 */
int		FileOpen(char* remote_resource,void** update);


/** FileClose():
 *  Effettua la chiusura del file mappato in memoria 
 *  @param update:	Puntatore alla risorsa mappata in memoria
 *  @param fd:	File descriptor della risorsa associata
 *  @return		Restituisce 0 in caso di insuccesso, altrimenti 1
 */
int		FileClose(void* update, int fd);





#endif
