/*
 * thread_functs.c
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
#include "thread_functs.h"


extern int oldoutptfd;

/** init_scheduler():
 *  Inizializza la struttura dati dello scheduler, predisponendo i 
 *  mutex ed inizializzando i valori:
 *  @param timbracartellino:	Puntatore alla Pool associata.
 */
int init_scheduler(SCHEDULING* timbracartellino) {
	init_list_head(&timbracartellino->planning);
	if (pthread_mutex_init(&timbracartellino->qlock,NULL)) return 1;
	if (pthread_cond_init(&timbracartellino->non_empty,NULL)) return 1;
	return 0;
}

/** thread_memento():
 *  Questa funzione effettua il controllo se esiste un lavoro da eseguire
 *  o meno
 *  @param x: parametro di tipo ARGS_ONE
*/
void* thread_memento( void *x)
{
	ARGS_ONE* todo = (ARGS_ONE*)x;
	while (FINEDIMONDO) 
	{
		JOB_QUEUE* steve; 
		pthread_mutex_lock(&todo->sched->qlock);
		/*NKprintf(9,"(%p) Waiting for new hanlder...\n", x);*/
		while (!todo->sched->qsize)
			pthread_cond_wait(&todo->sched->non_empty,&todo->sched->qlock);
		/*NKprintf(9,"(%p) Got New item...\n", x);*/
		if (todo->sched->qsize) 
			todo->sched->qsize--;
		steve = (JOB_QUEUE*)dequeue_job(&todo->sched->planning);
		/*NKprintf(9,"(%p) Dequeued: %p\n", x, (void*) steve);*/
		pthread_mutex_unlock(&todo->sched->qlock);
		/*NKprintf(9,"BujeBuje: running steve (%p)...\n", (void*) steve);*/
		run_job(todo->whoami, steve);
	}
}

/** main_add_job():
 *  Questa funzione effettua l'inserimento di dati nella coda dei JOB
 *  @param todo:	Puntatore allo scheduler
 *  @param func:	Puntatore alla funzione da eseguire
 *  @param args:	Puntatore agli argomenti della lista
 *  @param priorize:	Se settato a vero, inserisce in testa, altrimenti in coda
 */
void main_add_job( SCHEDULING* todo, void (*func)(), void* args, char priorize)
{
	JOB_QUEUE* val;
	
	pthread_mutex_lock(&todo->qlock);
		val = (JOB_QUEUE*)alloc_new_job(func,args);
	
		/*	la coda era vuota	*/
		if( !todo->qsize) 
		{
			todo->qsize++;
			pthread_cond_signal( &todo->non_empty);
		}
		/*	la coda NON era vuota	*/
		else
			todo->qsize++;
		
		/*	devo inserire in testa	*/
		if (priorize)
			push_job(&todo->planning,val);
		/*	non devo inserire in testa*/
		else
			enqueue_job(&todo->planning,val);
	pthread_mutex_unlock(&todo->qlock);
}
