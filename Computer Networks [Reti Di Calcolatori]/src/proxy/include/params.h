/*
 * params.h
 * This file is part of proxy
 *
 * Copyright (C) 2012 - Giacomo Bergami, Paolo de Luca
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
 * along with proxy; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


#ifndef __PARAMS__H__
#define __PARAMS__H__


	/*	Standard library	*/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>	
#include <sys/types.h>	
#include <sys/time.h>		
#include <netinet/in.h> 	
#include <errno.h>		
#include <arpa/inet.h>	
#include <obstack.h>


	/*	Project's library	*/
#include "macro.h"
#include "communication.h"


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/**	init_params()
 *		effettua il parsing dei parametri e setta l'indirizzo ip e porta da far
 *		utilizzare al proxy
 */
int init_params(int argc, char* argv[], struct proxy* params);

#define obstack_chunk_alloc malloc
#define obstack_chunk_free  free

/** IS_EOOPT():
 * Macro utilizzate per la definizione della lettura dei parametri */
#define IS_EOOPT(ptr) (((((ptr)->name)||((ptr)->has_arg)||((ptr)->flag)||((ptr)->val))==0) ? 1 : 0)

/** GETOPT_L():
 *  Calls getopt_long using mkshortopt() */
#define GETOPT_L(argc,argv,longk,optptr,w) getopt_long(argc,argv,w,longk,optptr)

/** WHILE_GETOPT_L():
 * Does the matching using mkshortopt()
 */
#define WHILE_GETOPT_L(argc,argv,longk,optptr,val,w) while(((val)=GETOPT_L(argc,argv,longk,optptr,w))!=-1)

/** LONGNAME():
 * Obtains the long name of the current pace
 */
#define LONGNAME(struc,opt)		((struc)[opt].name)

/** HAS_ARG:
 * The pointer to the value of the argument
 */
#define HAS_ARG				(optarg)

/** HAS_OTHER_ARGS():
 * At the end of the cycle, returns if there are some other arguments
 */
#define HAS_OTHER_ARGS(argc) 		(optind<(argc))

/** REMAINING_ARGS():
 * Counts the remaining argc(s)
 */
#define REMAINING_ARGS(argc)		((argc)-optind+1)

/** GET_NEXT_OPTIND():
 * If there is another arg, returns its char pointer
 */
#define GET_NEXT_OPTIND(argc,argv)	(HAS_OTHER_ARGS(argc) ? ((argv)[optind++]) : ((char*)0) )




/** START_CYCLE():
 * Defines the cycles to retrieve args. 
 */
#define START_CYCLE(argc,argv,longk)	do {\
						int option_index = 0;\
						int c;\
						char* allocando = mkshortopt(longk);\
						WHILE_GETOPT_L(argc,argv,longk,&option_index,c,allocando) { \
						switch(c) {	

/** END_CYCLE:
 *  Loop termination
 */
#define END_CYCLE			 } } free(allocando); } while(0)

#endif
