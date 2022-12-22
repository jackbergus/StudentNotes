#ifndef _THREAD__FUNC__H
#define _THREAD__FUNC__H


/*	Project's library	*/
#include 	"pthread_ext.h"
#include 	"uni_list.h"
#include 	"types.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define  FINEDIMONDO		42

/** init_scheduler():
 *  Inizializza la struttura dati dello scheduler, predisponendo i 
 *  mutex ed inizializzando i valori 
 */
int init_scheduler(SCHEDULING* timbracartellino);

/** thread_memento():
 *  Questa funzione effettua il controllo se esiste un lavoro da eseguire
 *  o meno
*/
void* thread_memento(void *todo);

/** main_add_job():
 *  Questa funzione effettua l'inserimento di dati nella coda dei JOB
 *  @param todo:	Puntatore allo scheduler
 *  @param func:	Puntatore alla funzione da eseguire
 *  @param args:	Puntatore agli argomenti della lista
 *  @param priorize:	Se settato a vero, inserisce in testa, altrimenti in coda
 */
void main_add_job(SCHEDULING* todo, void (*func)(), void* args, char priorize);

#endif
