/*
 * lett_scritt.h
 * This file is part of hashtable
 *
 * Copyright (C) 2012 - Giacomo Bergami
 *
 * hashtable is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * hashtable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hashtable; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */
#ifndef __LETT_SCRITT__H__
#define __LETT_SCRITT__H__


/*	Standard Library	*/
#include	<semaphore.h>
#include	<pthread.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


#define WREAD 	0
#define WWRITE	1

typedef unsigned int	ANYTYPE;

struct CS_struct {
	pthread_mutex_t attr;
};

#define P(x) 	sem_wait(x)
#define V(x)	sem_post(x)

/** init_struct():
 *  Inizializza la struttura dati passata per argomento,
 *  settando il numero dei lettori e degli scrittori,
 *  ed inizializzando il semaforo associato al dato 
 **/
void init_struct(struct CS_struct* data);


/** struct_do_read(): 
 *  Funzione di accesso mutuamente esclusivo in lettura,
 *  al termine della quale si può ottenere il dato richiesto.
 *  @param data: Indica la struttura dati sulla quale si effettua il lock
 *  @param external:	Indica un eventuale locking esterno da rilasciare
 * 						e di cui riappropriarsi in seguito
 **/
void struct_do_read(struct CS_struct* data);

/** struct_end_read():
 *  Funzione di rilascio della mutua esclusione in lettura sulla
 *  struttura dati 
 **/
void struct_end_read(struct CS_struct* data);


/** struct_do_write():
 *  Funzione di accesso in scrittura in mutua esclusione,
 *  al termine della quale si può scrivere il dato richiesto 
 **/
void struct_do_write(struct CS_struct* data);

/** struct_end_write():
 *  Funzione di rilascio della mutua esclusione in scrittura sulla
 *  struttura dati 
 */
void struct_end_write(struct CS_struct *data);


#endif 
