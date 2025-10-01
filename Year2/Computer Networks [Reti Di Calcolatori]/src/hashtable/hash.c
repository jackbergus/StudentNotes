/*
 * hash.c
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



#include	"hash.h"
#include 	<stdlib.h>
#define  SEED 13

void init_hashtbl(struct hash_tbl* p) {
	int i;
	srandom(time(NULL));
	for (i=0; i<HASH_SIZE; i++) {
		p[i].elements=p[i].value=0;
	}
}

unsigned int hashb(char *p, unsigned int MULT, unsigned int NHASH) {
	unsigned int h = 0;
	while (*p) h = MULT * h + *p++;
	return (h % NHASH);
}

unsigned int hash(char *p) {
	return (hashb(p,MULT1,NHASH1)+SEED*hashb(p,MULT2,NHASH2))%MOD;
}


int hash_exists(struct hash_tbl* tbl, char* p) {
	unsigned int hashv = hash(p);
	return (tbl[hashv].elements>0); 
}

void hash_insert(struct hash_tbl* tbl, char* p) {
	int hashv = (int)hash(p);
	tbl[hashv].elements++;
}

void hash_remove(struct hash_tbl* tbl, char* p) {
	int hashv = (int)hash(p);
	if (tbl[hashv].elements) tbl[hashv].elements--;
}

int hash_occupied(struct hash_tbl* tbl) {
	return (tbl->elements);
}
