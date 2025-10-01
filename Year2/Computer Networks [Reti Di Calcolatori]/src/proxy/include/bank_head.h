#ifndef __BANK__HEAD__H__
#define __BANK__HEAD__H__

#include "types.h"


/** cache_exists():
 *  Dato il pathening remoto, controlla che in cache esista la risorsa
 *  corrispondente, o che la risorsa non sia scaduta. Se scade, allora
 *  la risorsa viene cancellata e restituito un valore falso.
 *  @param remote_path:	Percorso della risorsa remota da richiedere al server
 *  @param dealloca: 	Se è settato a vero, non viene settato il valore 
 *                      local_path, altrimenti viene aggiornato il suo valore
 *  @param local_path:	Doppio puntatore a carattere: se passato per riferimento
 *                      un puntatore a carattere, aggiorna il suo valore con la
 *                      stringa remota; non viene modificato il valore se \a dealloca
 *                      è settato a vero.
 *  @return:		Restituisce 0 se non esiste, 1 se esiste e 2 se esiste ma è troncato
 */
char cache_exists(char* remote_path, int dealloca, char** local_path,char flag);

/** cache_init():
 *  Funzione per la inizializzazione della struttura della risorsa per la cache.
 *  @param Threads_Resource:	Risorsa locale associata al thread corrente. 
 *                              Può essere settata a NULL se existance_val == 2
 *  @param exsistance_val:	Valore di esistenza associato al file corrente 
 *                              (ottenuto da cache_exists)
 *  @param remote_path:		Nome della risorsa remota
 *  @return:			Restituisce o lo stesso puntatore alla risorsa
 *  				che è stata associata come parametro.
 */
RES *cache_init(RES *Threads_Resource, char existance_val, char* remote_path);

/** cache_init_socket():
 *  Data la struttura dati RES come argomento e data la dimensione del file, effettuo la divisione
 *  nell'acquisizone della risorsa. Ritorna 0 in caso di successo.
 *  @param Bart:		Struttura dati da aggiornare
 *  @param max_socks: 		Numero totale dei sockets utilizzati dal thread corrente
 *  @param no:			Numero del socket corrente, contando da 1
 *  @param remote_file_size:	Dimensione del file remoto
 */
int cache_init_socket(sockets* Bart, int max_socks, int no, int remote_file_size) ;


/**  cache_done():
 *  Questa funzione viene chiamata per effettuare l'unmapping della memoria.
 *  Deve essere effettuata garantendo precedentemente la mutua esclusione con la
 *  mutex_could_close --RES--.
 *  @param resource:	  	risorsa che deve essere chiusa
 *  @param do_free_not_unmap: 	identifica se, precedentemente, il file era 
 * 				stato troncato 
 */
void cache_done(RES* resource);

#endif
