#include <stdio.h>
#include <stdlib.h>
 
int main(void) {
	int pip[2];
	pipe(pip);
	
	if (fork())  {
		int readed = 0;
		read(pip[0],&readed,sizeof(readed));
		printf("%d\n", readed);
		return 0;
	} else {
		int writing = 50;
		close(pip[0]);
		write(pip[1],&writing,sizeof(writing));
		printf("quit...\n");
		return 0;
	}

}
