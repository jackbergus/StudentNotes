#ifndef __GEN_LISTS_H_
#define __GEN_LISTS_H_


/*	Standard Library	*/
#include <uni_list.h>



/* Lista di puntatori */
struct list_ptrint {
	void *valore;
	LIST elem;
};

#endif
