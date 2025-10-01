/*
 * filesystem.h
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
#ifndef __FS__H__
#define __FS__H__

/* Questo programma effettua l'LS di tutto le sottodirectory e
   quella corrente */
#define _GNU_SOURCE


/*	Standard library	*/
#include 	<sys/types.h>
#include 	<sys/mman.h>
#include 	<sys/mman.h>
#include 	<sys/stat.h>
#include 	<obstack.h>
#include 	<stdlib.h>
#include 	<string.h>
#include 	<unistd.h>
#include 	<libgen.h>
#include 	<dirent.h>
#include 	<stdio.h>
#include 	<fcntl.h>
#include 	<time.h>
#include 	<ftw.h>

/*	Project's Library	*/
#include 	"consts.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


char* obtain_local(char* remote);
void	recursiveDelete(char* cache);


/**  close_map():
 *   Effettua la chiusura della mappatura del file
 *   @param mem:	area di memoria da demappare 
 */
#define close_map(mem) do { 						\
				int val = munmap((void*)mem,FILE_SIZE);	\
				kassert(val>=0,  "Error unmapping file");\
		       }while(0)

/** cache_free():
 *  Funzione per cancellare dalla cache tutti i files e le cartelle contenute 
 */
#define cache_free() \
	recursiveDelete("cache")


#define FILE_SIZE			sizeof(RES_FILE)

#define NOW 				((unsigned int)(time(NULL)))
#define PAST(x) 			(NOW-((unsigned int)x))
#define EXPIRED(creation,expiry)	(((unsigned int)expiry)>PAST(creation))
	       
				/**************************************/
				/** Funzioni accessorie per la cache **/
				/**************************************/

/** file_exist():
 *  Verifica l'esistenza del file \a filename.
 *  Se il pathname esiste ed è un file, restituisce 1,
 *  se esso è una cartella restituisce 2, se non esiste 0. 
 *  @param filename:	file locale del quale controllare l'esistenza
 *  @param time:	Conterrà il tempo da Epoch della creazione del file
 *  @param is_blank:	Restituisce, se passato come argomento, se
 *                      il file è solamente troncato 
 *  @param retbuf:	Restituisce lo fstat del file, se esiste.
 *  @param pfd:		Possibile (o NULL) puntatore al FileDescriptor: in
 *			quel caso il file non verrà chiuso
 */
char	file_exists( char * filename, unsigned int *time, char*			is_blank, 
														  struct stat*	retbuf, 
														  int*			pfd);


char	folder_empty( char *dirname);



					/***************************/
					/** Funzioni fondamentali **/
					/***************************/


/** resource_exists():
 *  Questa funzione verifica se il path remoto esista o meno. In base al valore 
 *  del parametro \a deallocate, viene restituita o meno la stringa convertita 
 *  della risorsa locale corrispondente
 *  @param remote: 	path remoto
 *  @param deallocate:  indica se deallocare l'indirizzo locale della risorsa 
 *                      corrispondente
 *  @param result:	verrà scritto in questo puntatore l'esistenza o meno del
 *                      file
 *  @param time:	tempo di creazione della risorsa
 *  @param blank:	restituisce se il file è stato troncato o meno (e quindi
 *                      vuoto)
 *  @param retbuf:	restituisce (se non NULL) il puntatore all'fstat del 
 *                      file in cache
 *  @param pfd:		Possibile (o NULL) puntatore al FileDescriptor: viene
 *			passato un fd aperto.
 */
char*	resource_exists(char* remote, int deallocate, char* 		result,
													  unsigned int* time, 
													  char* 		blank, 
													  struct stat*	retbuf, 
													  int* 			pfd);


/**  re_new_resource():
 *  Data il path della cache, crea la nuova risorsa: se le cartelle intermedie
 *  non esistono, esse vengono create, ed il file finale viene troncato ed aperto
 *  alla dimensione base massima. Restituisce la data di creazione del file
 *  @param cache_path: 	Identifica il percorso locale della risorsa
 *  @param retbuf:	Identifica lo stat del file che è stato aperto.
 *			Se non NULL viene aggiornato.
 *  @param pfd:		Identifica il FileDescriptor del nuovo file. Se non
 * 			NULL viene aggiornato, altrimenti viene chiuso.
 */
void	re_new_resource( char* cache_path, struct stat *retbuf, int *pfd);


/** resource_remove():
 *  Questa funzione effettua l'unlink sul file definito, ed inoltre
 *  via via eventualmente cancella le cartelle genitori se queste 
 *  sono vuote.
 *  @param path:	Risorsa remota
 */
void	resource_remove( char* path);


#endif
