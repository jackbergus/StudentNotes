#ifndef GLOBAL_INIT_EXTERN
#define GLOBAL_INIT_EXTERN

#include "types.h"
#include "toclient.h"
#include "libhashtable.h"

			/**********************************/
			/* Variabili riguardanti i thread */
			/**********************************/
extern SCHEDULING		serverPool;
extern SCHEDULING		prefetchPool;
extern SCHEDULING		cachePool;

			/********************************/
			/* Variabile di stato del Proxy */
			/********************************/
extern struct proxy global;
extern ARGS_ONE prefetch_arg;

			/************************/
			/* Gestione della Cache */
			/************************/
extern HRES cache_hash_table;

#endif
