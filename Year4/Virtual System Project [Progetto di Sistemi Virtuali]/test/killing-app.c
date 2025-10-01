#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void) 
{
	int son;
	if (son = fork()) {
		printf("\033[22;32m ciao mondo\033[0m\n");
		waitpid(son,NULL,0);
		printf("\033[22;32m ciao mondo\033[0m\n");
		return 0;
	} else {
		/* FIGLIO */
		int nipote;
		int i=0;
	
		printf("FIGLIO CREATO %d\n",getpid());
/*
		sleep(2);
		if((nipote = fork()) > 0){
			printf("SONO IL FIGLIO %d E IL NIPOTE E'%d\n",getpid(), nipote);
			waitpid(nipote,NULL,NULL);
		}else if (nipote == 0){
			printf("SONO IL NIPOTE %d\n",getpid());
		}
		else printf("errore\n");
		printf("\033[22;32m hello-world 1' figlio %d\033[0m\n", getpid());
*/		
		for (; i<10; i++) {
			printf("\033[22;32m %d \033[0m\t", i);

			if ((nipote = fork()) > 0) {	//questo è il primo figlio che aspetta
				printf("\033[22;32m figlio %d   errno: %d \033[0m\n", getpid(), errno);
				waitpid(nipote,NULL,0);
			} else if (nipote == 0) {		//questo sarebbe il figlio di son, quindi nipote!
				printf("\033[22;32m nipote %d\033[0m\n", getpid());
				return 0;
			} else printf("\033[22;31m errore fork \033[0m");

		}
		printf("\033[22;32m finito 1' figlio\033[0m\n");
		return 0;
	}
}
