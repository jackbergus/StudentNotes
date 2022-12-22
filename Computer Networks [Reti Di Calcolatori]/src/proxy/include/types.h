#ifndef __TYPES_H__
#define __TYPES_H__


	/*	Standard library	*/
#include 	<arpa/inet.h>
#include	<pthread.h>


	/*	Project's library	*/
#include	"consts.h"




/** request:
 * Struttura dati per la gestione della richiesta lato client 
 */
typedef struct 
{
	/**@{*/
	char 			TYPE;			/**< Tipo di richiesta effettuata */
	char			PATH[MAX_PATH];	/**< Percorso della risorsa */
	unsigned int	rangelow;		/**< Eventuale lower-rage */
	unsigned int	rangehight;		/**< Eventuale higher-range */
	/**}@*/
} request;


		/***********************************************/
		/* Strutture dati per il passaggio degli args* */
		/***********************************************/

/** msg:
 *  Struttura per il passaggio dei parametri alle funzioni 
 *  di gestione delle pool.
 */
typedef struct 
{	
	/**@{*/
	int		sock;	/**<	il fd che rappresenta la connessione col client	*/
	request	rq;		/**<	la richiesta( parsata) fatta dal client	*/
	/**}@*/
} msg;


/** reslist:
 *  Struttura per il passaggio dei parametri delle richieste di
 *  prefetch, inesistenti in cache
 */
struct reslist
{
	/**@{*/
	int   prefetch_num;	/**< Livello attuale di prefetching */
	char* remote_path;	/**< Stringa indicante la risorsa da ottenere */
	/**}@*/
};

/* LIST:
 * Definizione di una struttura di dati lista minimale 
 */
typedef struct __mialista__
{
	/**@{*/
	struct __mialista__* next;  /**<	Puntatore al successivo	*/
	/**}@*/
} LIST;


/** JOB:
 *  Questa struttura dati contiene il lavoro che deve essere effettuato dal thread in questione.
 *  Il thread in particolare dovrà eseguire una data funzione, dove è presente una nostra
 *  procedura, con degli argomenti che verranno passati successivamente alla sua chiamata
 */
typedef struct
{
	/**@{*/
	void (*procedura)();	/**< Procedura associata alla richiesta */
	void  *args;			/**< Argomenti da passare alla procedura */
	/**}@*/
} JOB;




/** JOB_QUEUE:
 *  Questa struttura dati serve per implementare una coda di lavori da effettuare, che verranno
 *  messi all'interno della struttura dati in questioneRetorica_musicale
 */
typedef struct
{
	/**@{*/
	LIST 	head;	/**< Testa della lista */
	JOB		todo;	/**< Lavoro da effettuare */
	/**}@*/
} JOB_QUEUE;




/** SCHEDULING:
 *  Questa struttura dati serve per ricordarsi, all'interno del programma, lo stato corrente
 *  dell'esecuzione dei lavori all'interno del proxY.
 */
typedef struct 
{	
	/**@{*/
	LIST	 			planning;	/**<	Lista dei JOB che devono essere svolti dai thread correnti	*/
	pthread_mutex_t 	qlock;		/**<	Lock per l'accesso alla lista	*/
	pthread_cond_t		non_empty;	/**<	Condizione di coda non vuota    */
	unsigned short int	qsize;		/**<	Dimensione della coda dei dati	*/ 
	/**}@*/
} SCHEDULING;



/** ARGS_ONE
 *  Questa struttura dati consente di specificare, ai thread che usano
 *  serverPool, a quale indice di cacheRes devono far riferimento.
 */
typedef struct
{
	/**@{*/
	SCHEDULING*	sched;	/**< Puntatore allo scheduling associato */
	char		whoami; /**< Identificatore numerico del thread */
	/**}@*/
} ARGS_ONE;


/** struct connection:
 *  Struttura dati contenente lo stato attuale della connessione 
 */
struct connection
{
	/**@{*/
	int 	fd_client;		/**< Eventuale File Descriptor del client richiedente */
	int 	fd_server;		/**< Eventuale File Descriptor del server al quale si richiede */
	int 	tentativi;		/**< Conteggio dei tentativi effettuati */
	int 	expiry;			/**< Tempo di expire associata alla risorsa in download */
	char 	PATH[MAX_PATH];	/**< Stringa indicante il percorso della richiesta */
	char 	sIP[16];		/**< Indirizzo IP del server al quale si inoltra la richiesta */
	char 	sPort[7];		/**< Porta del server al quale si inoltra la richiesta */
	/**}@*/
};


/** struct proxy:
 *  Struttura dati dove sono raccolte le informazioni per la connessione lato
 *  client 
 */
struct proxy
{
	/**@{*/
	struct sockaddr_in meStesso;
	int 		fd;		/**<	Fd del proxy lato client	*/
	int 		level;	/**<	Livello di prefetch	*/
	in_addr_t 	ip;		/**<	IP del proxy	*/
	int 		port;	/**<	porta del proxy	*/
	/**}@*/
};


#endif
