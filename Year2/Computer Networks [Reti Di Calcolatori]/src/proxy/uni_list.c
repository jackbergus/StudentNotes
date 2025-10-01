/*
 * uni_list.c
 * This file is part of proxy
 *
 * Copyright (C) 2012 - Giacomo Bergami
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
#include "uni_list.h"


extern int oldoutptfd;

/** new():
 *  Creazione di un nuovo elemento lista 
 */
LIST* new(void) {
	LIST* tmp=malloc(sizeof(LIST));
	tmp->next=NULL;
	return tmp;
}



/** dequeue():
 *  Disaccoda il primo elemento dalla lista 
 */
LIST* dequeue(LIST** ptr) {
	LIST* tmp = *ptr;
	if (*ptr) {
		*ptr=(((*ptr)->next));
		return tmp;
	} else return *ptr;
}



/** remove_elem():
 *  rimuove un dato elemento dalla lista */
LIST* remove_elem(LIST* ptr, LIST* elem) {
	LIST* tmp = ptr->next;
	LIST* prev = ptr;
	
	if (ptr) {
		while (prev->next!=elem) { 
			prev = tmp;
			tmp  = tmp->next;
			if (!tmp) return NULL;
		}
		prev->next = prev->next->next;
		return tmp;
	} else return ptr; 
}



/** enqueue():
 *  Accoda un elemento particolare della lista 
 */
void enqueue(LIST* ptr, LIST *include) {
	LIST *tmp, *prev;
	tmp = ptr;
	do 
		prev=tmp; 
	while ((tmp=tmp->next));
	prev->next = include;
}


/** push():
 *  Effettua l'inserimento con priorità degli elementi
 */
void push(LIST* ptr, LIST *include) {
	include->next = ptr->next;
	ptr->next = include;
}



			/*******************************/
			/** LISTE PER LA CODA DEI JOB **/
			/*******************************/


/** init_job_queue():
 *  Creazione di un nuovo elemento lista 
 */
void init_job_queue(LIST* tmp) {
	tmp->next=NULL;
}



/** dequeue_job_queue():
 *  Disaccoda il primo elemento dalla lista ed esegue il primo lavoro
 */
JOB_QUEUE* dequeue_job(LIST* ptr) {
	JOB_QUEUE* ret;
	container_of2(dequeue(&ptr->next),JOB_QUEUE,head, ret);
	return ret;
}

void run_job(int k, JOB_QUEUE* tmp) {
	void (*fun)() = tmp->todo.procedura;
	void *args    = tmp->todo.args;
	free(tmp);
	fun(k,args); 
}

/** remove_job():
 *  rimuove un dato elemento dalla lista */
JOB_QUEUE* remove_job(LIST* ptr, JOB_QUEUE* elem) {
	JOB_QUEUE* ret;
	LIST* ptr2 = remove_elem(ptr,&elem->head);
	container_of2(ptr2,JOB_QUEUE,head, ret);
	free(ptr2);
	return ret;
}


/** alloc_new_job():
 *  Dato un puntatore generico a funzione e un puntatore ad una struttura
 *  dati dell'argomento che la funzione accetta, viene generato un nuovo 
 *  elemento della lista 
 */
JOB_QUEUE* alloc_new_job(void (*procedura)(), void* args) {
	JOB_QUEUE* toret = (JOB_QUEUE*)malloc(sizeof(JOB_QUEUE));
	init_job_queue(&toret->head);
	toret->todo.procedura = procedura;
	toret->todo.args      = args;
	return toret;
}



/** enqueue_job():
 *  Accoda un elemento particolare della lista 
 */
void enqueue_job(LIST* ptr, JOB_QUEUE* include) {
	enqueue(ptr,&include->head);	
}



/** push_job():
 *  Mette in testa della lista l'elemento 
 */
void push_job(LIST* ptr, JOB_QUEUE* include) {
	push(ptr,&include->head);
}

/** exists_job_res():
 *  Effettua la verifica che sia già stata inoltrata 
 *  tale richiesta di download remoto 
 */
char exists_job_res(LIST* ptr, char* path) {
	LIST *tmp, *prev;
	tmp = ptr->next;
	
	if (tmp)
	do {
		struct reslist *myptr;
		JOB_QUEUE      *ptrs;
		prev=tmp;
		container_of2(prev,JOB_QUEUE,head,ptrs);
		myptr = ptrs->todo.args;
		
		if ((!myptr)) {
			return (char)0;
		} else if (!myptr->remote_path) {
			return (char)0;
		}
		if (strcmp(myptr->remote_path,path)==0) {
			printf("Request %s already in prefetchPool: skipping...\n", path);
			return (char)1;
		};
	} while ((tmp=tmp->next));
	return (char)0;
}

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
	int bufferlow, int bufferhight) {
	
	printf("Level: %d<=%d ??\n", curr_pos,MAX_POS);
	if (curr_pos<=MAX_POS) {
	
		char *next;
		char *ret;
		char *base_text;

		printf("here\n");	
		base_text = buffer;
		while ((ret = get_resource_alloc(base_text,&next))) {
			if (exists_job_res(&prefetchPool.planning,ret)) {
				free(ret);
				ret = NULL;
			}
			if ((ret)) {
				struct reslist* myptr = (struct reslist*)\
							malloc(sizeof(struct reslist));
				myptr->remote_path = ret;
				myptr->prefetch_num = curr_pos + 1;
				printf("\033[31mAdding in prefetch: %s\033[0m\n",ret); 
				main_add_job(&prefetchPool, func, myptr,0);
			}
			base_text = next;
		}
	}
	
}


