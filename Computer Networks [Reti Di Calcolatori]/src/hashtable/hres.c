/*
 * hres.c
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


#include	"hres.h"

int parseHead_time(char* buf);

/** init_hres():
 *  Inizializzazione della struttura dati
 */
void init_hres(HRES* ptr) {
	int i;
	for (i=0; i<MAX_HRES; i++) {
			init_hashtbl(&ptr->hashtable[i]);
			init_struct(&ptr->lettscrit[i]);
	}
}

/** is_inhash():
 *  Controlla se il file è presente in cache, e se esiste effettivamente
 *  all'interno del filesystem
 */
int is_inhash(HRES* phash, char* remote_file) {
	int i = hash(remote_file);
	int pos = i;
	char* local = obtain_local(remote_file);
	
	struct_do_write(&phash->lettscrit[pos]);
	if ((phash->hashtable[i].elements)==0) i = -1;
	if (i!=-1) {
		unsigned int tempo;
		if (!file_exists(local,&tempo,NULL,NULL,NULL)) i = -1;
		else {
			char *toret;
			int expiry;
			int fd = FileOpen(remote_file,(void*)&toret);
			if (fd==-1) i = -1;
			else {
				expiry = parseHead_time(toret);
				FileClose(toret,fd);
				if (expiry < (time(NULL)-tempo)) {
					hash_remove(phash->hashtable,remote_file);
					resource_remove(local);
					i=-1;
				}
			}
		}
	}
	struct_end_write(&phash->lettscrit[pos]);
	
	free(local);
	return i;
}

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
int handle_file_filesystem(HRES* phash, char* remote_file, void* res, int creat_oth_delete) {
	char* local;
	int i, pos;
	
	i = hash(remote_file);
	printf("catched: %d\n", i);
	local = obtain_local(remote_file);
	
	struct_do_write(&phash->lettscrit[i]);
	if (creat_oth_delete) {
		if (!file_exists(local,NULL,NULL,NULL,NULL)) {
			if (!FileCreate(remote_file,res)){
				pos = -1;
			} else {
				hash_insert(phash->hashtable,remote_file);
				pos = i;
			}
		}
	} else {
		local = obtain_local(remote_file);
		resource_remove(local);
		hash_remove(phash->hashtable,remote_file);
		pos = -1;
		free(local);
	}
	struct_end_write(&phash->lettscrit[i]);	
	
	
	return pos;
}

int map_cached_file(HRES* hash, char* remote_file,void** toret) {
	int pos = is_inhash(hash, remote_file);
	char* local;
	int i, fd;
	
	if (pos==-1) return pos;
	else i = pos;
	local = obtain_local(remote_file);
	
	/* INIZIO DELLA LETTURA */
	struct_do_read(&hash->lettscrit[i]);
	fd = FileOpen(remote_file,toret);
	if (fd==-1) struct_end_read(&hash->lettscrit[i]);
	free(local);
	return fd;	
}

void close_cached_file(HRES* phash, char* remote_file, void* toclose, int fd) {
	int pos = hash(remote_file);
	char* local;
	int i;
	
	if (pos==-1) return;
	i = pos;
	local = obtain_local(remote_file);
		
	/* FINE DELLA LETTURA */
	FileClose(toclose,fd);
	struct_end_read(&phash->lettscrit[i]);
	free(local);
}

/** parseHead_time()
 *  Semplificazione della funzione di parsing del file, per ottenerne solamente
 *  Il valore di expire
 */
int parseHead_time(char* buf) {

	int n, resno, len_, low_, hight_;
	unsigned int 	expire_ = 0;
	n = resno = len_  = low_ = hight_ = 0;
	
	if (!buf) return 0;	
	
	/*resource *tmp; */
	n = sscanf(buf,"%d\n\n", &resno);
	
	if ((n)&&((resno==404)||(resno==405)||(resno==403)||(resno==402))) {
		return 0;
	} else {
		/* Gestione dei casi con argomenti Len ed Expire */
		n = sscanf(buf, "%d\nLen %d\nExpire %u\n\n*", &resno, &len_, &expire_);
		if ((n==3)&&((resno==202)||(resno==200))) {
			return expire_;
		} else {
			/* Ultimo caso rimasto: formato 201 */
			n = sscanf(buf, "201\nRange %d-%d\nExpire %u\n\n*", &low_, &hight_, &expire_);
			if (((n==2)&&(expire_)&&(!hight_)&&(low_))||(n==3)) {
				return expire_;
			} else
				/* La risposta non è ben formata */
			 	return 0;		
		}
	}
}
