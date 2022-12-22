#ifndef __UNI_LIST__H__
#define __UNI_LIST__H__



	/*	Standard library	*/
#include 	<stddef.h>
#include 	<stdlib.h>
#include 	<stdio.h>


	/*	Project's library	*/
#include 	"libhashtable.h"
#include 	"thread_functs.h"
#include	"types.h"
#include 	"init_globals.h"
#include 	"communication.h"






/** init_list_head():
 *  Inizializzazione della struttura dati di tipo lista 
 */
#define init_list_head(head) \
	((LIST*)head)->next = NULL



/** container_of2():
 *  Ottiene il contenitore della struttura dati puntatore 
 */
#define container_of2(ptr, type, member, ret) 			\
	/*do {		*/					\
	/*const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	ret = (type *)( (char *)__mptr - offsetof(type,member) );*/\
	ret = (type *)( (char *)ptr - offsetof(type,member) )/*;} while(0)*/



LIST* new_list(void);

LIST* dequeue(LIST** ptr);

void enqueue(LIST* ptr, LIST* include);

LIST* remove_elem(LIST*ptr, LIST* elem);

	/********************************/
			/* FUNZIONI DI GESTIONE DEI JOB */
			/* 	(uni_list.c)            */
			/********************************/


/** init_job_queue():
 *  Creazione di un nuovo elemento lista 
 */
void init_job_queue(LIST* tmp);



/** dequeue_job_queue():
 *  Disaccoda il primo elemento dalla lista ed esegue il primo lavoro
 */
JOB_QUEUE* dequeue_job(LIST* ptr);



void run_job(int k, JOB_QUEUE* tmp);



/** remove_job():
 *  rimuove un dato elemento dalla lista */
JOB_QUEUE* remove_job(LIST* ptr, JOB_QUEUE* elem);



/** alloc_new_job():
 *  Dato un puntatore generico a funzione e un puntatore ad una struttura
 *  dati dell'argomento che la funzione accetta, viene generato un nuovo 
 *  elemento della lista 
 */
JOB_QUEUE* alloc_new_job(void (*procedura)(), void* args);



/** enqueue_job():
 *  Accoda un elemento particolare della lista 
 */
void enqueue_job(LIST* ptr, JOB_QUEUE* include);



/** push_job():
 *  Mette in testa della lista l'elemento 
 */
void push_job(LIST* ptr, JOB_QUEUE* include);



/** exists_job_res():
 *  Effettua la verifica che sia già stata inoltrata 
 *  tale richiesta di download remoto 
 */
char exists_job_res(LIST* ptr, char* path);



/** update_list():
 *  Dato un buffer dove è contenuta la parte di risorsa che è stata scaricata,
 *  aggiunge alla lista res_list le risorse solamente se queste non sono già
 *  presenti in cache o se non stanno per essere scaricate.
 *  @param buffer:	buffer dove è contenuta tutta la risorsa remota
 *  @param func:	funzione associata all'esecuzione della risorsa
 *  @param curr_pos:	identifica il livello corrente nell'albero di download 
 *                      di prefetching
 *  @param MAX_POS:	identifica la posizione massima invalicabile, oltre la 
 *                      quale non si effettuerà più download
 *  @param bufferlow:	Dimensione bassa del buffer da leggere
 *  @param bufferhight: Dimensione alta del buffer da leggere
 */
void update_list(char* buffer, void (*func)(), int curr_pos, int MAX_POS,\
	int bufferlow, int bufferhight);




#endif
