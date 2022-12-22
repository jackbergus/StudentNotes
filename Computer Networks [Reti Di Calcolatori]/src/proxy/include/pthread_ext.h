#ifndef __PTHREAD_TYPE__H__
#define __PTHREAD_TYPE__H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** mutex_init():
 *  Macro per la semplificazione della inizializzazione dei mutex 
 */
#define mutex_init(x)	do {	 				\
			 int rtn  = pthread_mutex_init(x,NULL);	\
			 if (rtn) {\
				fprintf(stderr,"Error while init mutex: %s\n", (char*)strerror(rtn));	\
				exit(1);			\
			 }  					\
			} while(0)


/** cond_init():
 *  Funzione per la semplificazione della inizializzazione delle condizioni 
 */
#define cond_init(x)	do {	 				\
			 int rtn = pthread_cond_init(x,NULL); 	\
			 if (rtn) { \
				fprintf(stderr,"Error while init condition: %s\n", (char*)strerror(rtn));	\
				exit(1);			\
			 }  					\
			} while(0)


#endif
