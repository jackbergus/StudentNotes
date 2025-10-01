/*
 * hash.h
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
#ifndef __HASH__H_
#define __HASH__H_


/*	Standard Library	*/
#ifndef		_BSD_SOURCE
#define		_BSD_SOURCE
#endif
#include 	<time.h>
#include 	<stdlib.h>
#include 	<stdio.h>


/*	Project's Library	*/
#include 	"consts.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/** struct hash_tbl:
 *  Struttura dati di hashing 
 */
struct hash_tbl {
	unsigned int elements;
	unsigned int value;
};

/** init_hashtbl():
 * Inizializzazione della hashtable 
 */
void init_hashtbl(struct hash_tbl* p);

/** hashb():
 *  Funzione di base per lo hashing 
 */
unsigned int hashb(char *p, unsigned int MULT, unsigned int NHASH);

/** hash():
 *  Funzione di hash 
 */
unsigned int hash(char *p);


/** hash_exists():
 *  Verifica che una stringa sia presente all'interno della tavola 
 */
int hash_exists(struct hash_tbl* tbl, char* p);

/** hash_insert():
 *  Effettua l'inserimento con collisioni 
 */
void hash_insert(struct hash_tbl* tbl, char* p);

/** hash_remove():
 *  Effettua la rimozione di un elemento o di una collisione
 */
void hash_remove(struct hash_tbl* tbl, char* p);

/** hash_occupied():
 *  Verifica se l'elemento è occupato 
 */
int hash_occupied(struct hash_tbl* tbl);

#endif
