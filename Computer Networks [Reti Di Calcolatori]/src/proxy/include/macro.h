#ifndef __MACRO__H__
#define __MACRO__H__


/*	Standard library	*/
#include	<sys/socket.h>
#include	<sys/types.h>
#include 	<sys/stat.h>
#include 	<unistd.h>
#include 	<string.h>
#include 	<stdio.h>
#include 	<errno.h>
#include 	<ctype.h>
#include 	<fcntl.h>

/*	Project's Library	*/
#include	"consts.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**	CPrintf()
 *		Wrapper for a colored printf()
 *
 *		NOTES:
 *			--background won't change
 */
/*	CPrintf( int foreground_color, ...)	*/
#define CPrintf( fg_color, ...)													\
{																				\
	/*	set color mode	*/														\
	printf( "%s%d;%d%s", START_COLOR, BOLD, (fg_color),	END_COLOR);				\
																				\
	/*	print original message	*/												\
	printf( __VA_ARGS__);														\
																				\
	/*	unset color mode	*/													\
	printf( "%s", RESET_COLOR);													\
}

		/***********************/
		/* PREPROCESSOR MACROS */
		/***********************/
#define ONCONDRET1(cond) if (cond) return 1


/** KASSERT():
 *  Funzione di asserzione: se la condizione (a) non è verificata,
 *  stampa il file e la linea dell'occorrenza dell'errore. I parametri
 *  successivi sono come quelli della printf, ed accetta quindi un 
 *  formato con le variabili.
 *  @param a:	condizione che non si deve verificare per generare l'errore
 */
#ifndef DEBUG
	#ifndef TESTING
	#define kassert(a,...) \
			while(0)
	#else
	#define kassert(a,...) 														\
			do { 																\
				if (!(a)) { 													\
					printf("DBG ERROR errno %d at file %s line %d:\n",errno,__FILE__,__LINE__); fflush(NULL); \
					printf(__VA_ARGS__); 		 fflush(NULL);					\
					exit(1); 													\
				}  																\
			} while(0)
	#endif
#else
#define kassert(a,...) 															\
		do { 																	\
			if (!(a)) { 														\
				printf("DBG ERROR errno: %d at file %s line %d:\n",errno,__FILE__,__LINE__); fflush(NULL); \
				printf(__VA_ARGS__);  fflush(NULL);								\
				exit(1); 														\
			}  																	\
		} while(0)
#endif



/**	SET_TIMEOUT()
 *  Per automatizzare il settaggio del timeout
 *  @param timeout:	indica il tempo da resettare
 *  
 */	

/* SET_TIMEOUT(struct timeval* timeout) */
#define SET_TIMEOUT(timeout){													\
	(timeout)->tv_sec		=	MAX_TIME;										\
	(timeout)->tv_usec		=	MAX_uTIME;										\
}

#endif 
