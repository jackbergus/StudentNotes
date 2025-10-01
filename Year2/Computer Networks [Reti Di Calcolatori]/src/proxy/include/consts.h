#ifndef __MY__CONSTS__H__
#define __MY__CONSTS__H__



/*	Costanti di protocollo	*/
#define INTERVAL_NOT_FOUND		405
#define NOT_FOUND				404
#define WRONG_REQUEST			403
#define UNKNOWN_ERROR			402
#define INFO					202
#define OK						200
#define OK_RANGE				201

#define OK_INFO					202

#define MAXREF	  				10
#define MAXIDREF				10
#define MAXCONN					5


/*	Constanti del tempo	*/
#define MAX_uTIME 				0
#define MAX_TIME 				3
#define MAX_TIMESELECT			4


/*	Constanti del proxy	*/
#define PROXY_PORT				55554
#define PROXY_IP				"127.0.0.1"
#define NO_MORE_THREAD  (THREAD_COUNT==MAXCONN) 


#define LIMIT_FILE 			5000
/* lungezza massima di un percorso (3° parte di una richiesta miniHttp)	*/	
#define MAX_PATH 			2048
/* dimensione massima di un file	*/
#ifndef MAX_FILE
	#define MAX_FILE 		5001	
#endif
#define MAX_BUFF 			5001
#define MAXREQUEST 			MAX_FILE


/* Costanti varie */
#define GET 				1
#define INF 				2


#define UNKNOWN_ERROR_SIZE	6	/*	402\n\n\0 */


#define SOCKET_ERROR		-1



/*	ANSI( CSI) color macros	*/
	#define	START_COLOR			"\033["
	#define END_COLOR			"m"	
	#define RESET_COLOR			"\033[0;37m" 

	/*	effects	*/
	#define BOLD				1

	/*	foreground colors	*/
	#define BLACK				30
	#define RED					31	
	#define GREEN				32
	#define YELLOW				33
	#define BLUE				34
	#define MAGENTA				35
	#define	CYAN				36
	#define WHITE				37
/*---------------------------*/


#endif
