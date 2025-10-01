/*
 * hres.h
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
#ifndef __HRES_H__
#define __HRES_H__


/*	Standard Library	*/
#include	<sys/types.h>
#include	<pthread.h>


/*	Project's Library	*/
#include	"lett_scritt.h"
#include	"filesystem.h"
#include 	"fsys.h"
#include 	"hash.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


typedef struct {
	struct hash_tbl 	hashtable[MAX_HRES];
	struct CS_struct 	lettscrit[MAX_HRES];
} HRES;

/** init_hres():
 *  Inizializzazione della struttura dati
 */
void init_hres(HRES* ptr);


/** is_inhash():
 *  Controlla se il file è presente in cache, e se esiste effettivamente
 *  all'interno del filesystem
 */
int is_inhash(HRES* phash, char* remote_file);


/** handle_file_filesystem():
 *  Funzione per la gestione controllata degli elementi del filesystem
 *  @param hash:		Struttura dati di hashing
 *  @param remote_file:	Indicazione del file remoto di risorsa
 *  @param res:			Indica la risorsa contentente le informazioni,
 * 						non utilizzata per l'eliminazione del file
 *  @param creat_oth_delete:	Indica se il valore è maggiore di zero,
 * 						di creare il file, altrimenti di cancellarlo.
 *  @return La funzione restituisce comunque -1 in caso di cancellazione,
 * 			la posizione nella htable (0 based) in caso di creazione
 * 			con successo, altrimenti -1
 */
int handle_file_filesystem(HRES* hash, char* remote_file, void* res, int creat_oth_delete);


int map_cached_file(HRES* hash, char* remote_file,void** toret);


void close_cached_file(HRES* hash, char* remote_file, void* toclose, int fd);
#endif
