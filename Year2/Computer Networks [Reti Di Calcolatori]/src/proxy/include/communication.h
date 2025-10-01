#ifndef __COMMUNIC__H__
#define __COMMUNIC__H__

#define _GNU_SOURCE


/*	Standard Library	*/
#include <stdio.h>
#include <stdlib.h>


/*	Project's Library	*/
#include "macro.h"
#include "types.h"


char* get_resource_alloc(char* text, char** next);

/**
 *  distance()
 *
 * MACRO DI CALCOLO
 * ----------------
 * Permette di effettuare il calcolo in quale posizione si trova ptr2 rispetto 
 * a ptr1, sapendo che sono puntatori con la stessa dimensione in memoria
 */
#define distance(ptr1,ptr2) ((!(sizeof(ptr1)==sizeof(ptr2))) ? -1 : (((ptr2)-(ptr1))/sizeof(*ptr1)))


int dorequest(char* request, int TYPE, char* path, int low, int hight) ;
int doreply(char *reply, int TYPE, int len, int empire, int low, int hight, char* content);
request* parseRequest(char* buf, int len);
int parseHead(char* buf, int buflen, int* len, unsigned int* expire, int* low, int* hight, char** cont);
char* getIP(char* URL, char* dest);
char* getPort(char* URLp, char* dest);



#endif
