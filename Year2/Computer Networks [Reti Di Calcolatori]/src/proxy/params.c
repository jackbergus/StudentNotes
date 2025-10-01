/*
 * params.c
 * This file is part of proxy
 *
 * Copyright (C) 2012 - Giacomo Bergami, Paolo de Luca, Elena Fabbri
 *
 * proxy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * proxy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with proxy. If not, see <http://www.gnu.org/licenses/>.
 */


#include "params.h"


/* Variabili globali che serviranno
 * anche al processo figlio, per
 * sapere sul da farsi */

char* mkshortopt(struct option* longopt);

/** help():
 *  Stampa in output l'help sull'utilizzo del programma
 */
void help() 
{
	printf("\n\n\tproxy (c) 2012\n");
	printf("\t--------------\n");
	printf("\t--tree -N:\t Setta il livello di profondità di prefetch\n");
	printf("\t--port -p:\t Setta il valore di connessione della porta (def: 5050)\n");
	printf("\t--ip   -i:\t Setta il valore di connessione di IP (def: 127.0.0.1)\n");
	printf("\t--help -h:\t Visualizza questa schermata\n\n\n");
}


/** init_params():
 *  Effettua il parsing dei parametri accettati dal main.
 *  @param args: lunghezza dei parametri
 *  @param argv: Parametri di lunghezza dichiarata nel parametro precedente
 *  @param params: Puntatore allo stato di connessione lato client 
 */
int init_params(int argc, char* argv[], struct proxy* params)
{
    char gotport=0;
    char gotip=0;
    
    struct option lo[] = {
	{"tree", required_argument,0,'N'},/*Imposto il valore di prefetch*/
	{"port", required_argument,0,'p'},/*Ottengo il valore della porta*/
	{"ip",   required_argument,0,'i'},/*Ottengo il valore dell'IP*/
	{"help", no_argument,      0,'h'},/*Ottengo informazioni*/
	{0,0,0,0}	
    };
	
    /** INIZIO DEL RICONOSCIMENTO DEI PARAMETRI **/
    START_CYCLE(argc,argv,lo)
		case 'N':
			params->level = (int)strtol((char*)HAS_ARG,(char**)NULL,10);
			break;
		case 'p':
			params->port = atoi(HAS_ARG);
			if (params->port <= 0 || params->port > 65535) {
				printf("Error: using default port...\n");
				fflush(NULL);
			} else gotport = (char)1;
			break;
		case 'i':
			if (inet_pton(AF_INET, HAS_ARG, &params->ip) != 1) {
				printf("Error: using default IP....");
				fflush(NULL);
			} else {
				printf("Using IP: %s\n",HAS_ARG);
				gotip = (char)1;
				params->ip = inet_addr(HAS_ARG);
			}
			break;
		default:
			help();
			return 1;
    END_CYCLE;
    

    /*	non è stato specificato l'indirizzo ip	*/
    if( !gotport) 
    {
    	printf("Using default port: %d\n",PROXY_PORT);
    	params->port = PROXY_PORT;
    }
    else 
    	printf("Using port: %d\n",params->port);
    /*	non è stato specificato l'indirizzo ip	*/
	if( !gotip)
	{
		printf("Using default IP: %s\n",PROXY_IP);
		inet_pton(AF_INET, PROXY_IP, &params->ip);
	} 
   return 0;
}


/** mkshortopt():
 *  Given the struct option data structure, it returns the correspondent
 *  shortopts char, by allocation of a string 
 */  
char* mkshortopt(struct option* longopt) {
	int 		cnt_chars=0;
	struct option 	*tmp;
	char 		*toret, *ctmp;
	
	
	/*Counts the chars to allocate*/
	tmp=longopt;
	while (!IS_EOOPT(tmp)) {
		if (!(tmp->flag)) {
			switch (tmp->has_arg) {
				case no_argument:
					cnt_chars++;
					break;
				
				case required_argument:
					cnt_chars+=2;
					break;
				
				case optional_argument:
					cnt_chars+=3;
					break;
				
				default:
					kassert(0,"mkshortopt: IS_EOOPT\n");
			
			}
		}
		tmp++;
	}
	tmp=longopt;
	cnt_chars++;
	ctmp=toret=(char*)malloc(sizeof(char)*cnt_chars);
	
	
	/* Creation of the substirng */
	while (!IS_EOOPT(tmp)) {
		if (!(tmp->flag)) {
			switch (tmp->has_arg) {
				case no_argument:
					*ctmp++=(char)tmp->val;
					break;
				
				case required_argument:
					*ctmp++=(char)tmp->val;
					*ctmp++=':';
					break;
				
				case optional_argument:
					*ctmp++=(char)tmp->val;
					*ctmp++=':';
					*ctmp++=':';
					break;
				
				default:
					kassert(0,"mkshortopt: IS_EOOPT\n");
			
			}
		}
		tmp++;
	}
	*ctmp++='\0';
	
	return toret;
	
}		

