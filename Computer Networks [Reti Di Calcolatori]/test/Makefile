GCCFLAGS= -Wall -Wunused -ansi -pedantic -ggdb 
LINKERFLAGS=-lpthread -lm

all:	Server1.exe Server2.exe Client1.exe Client2.exe
	

Client1.exe: Client1.o Util.o
	gcc -o Client1.exe ${GCCFLAGS} ${LINKERFLAGS} Client1.o Util.o 

Client1.o: Client.c Util.h 
	gcc -c ${GCCFLAGS} -o Client1.o Client.c

Client2.exe: Client2.o Util.o
	gcc -o Client2.exe ${GCCFLAGS} ${LINKERFLAGS} Client2.o Util.o 

Client2.o: Client.c Util.h 
	gcc -c ${GCCFLAGS} -DCLIENT2 -o Client2.o Client.c

Server1.exe: Server1.o Util.o
	gcc -o Server1.exe ${GCCFLAGS} ${LINKERFLAGS} Server1.o Util.o 

Server1.o: Server.c Util.h 
	gcc -c ${GCCFLAGS} -o Server1.o Server.c

Server2.exe: Server2.o Util.o
	gcc -o Server2.exe ${GCCFLAGS} ${LINKERFLAGS} Server2.o Util.o 

Server2.o: Server.c Util.h 
	gcc -c ${GCCFLAGS} -DSOLORANGE -o Server2.o Server.c

Proxy.exe: Proxy.o Util.o
	gcc -o Proxy.exe ${GCCFLAGS} ${LINKERFLAGS} Proxy.o Util.o 

Proxy.o: Proxy.c Util.h 
	gcc -c ${GCCFLAGS} Proxy.c

Util.o: Util.c Util.h 
	gcc -c ${GCCFLAGS} Util.c

clean:	
	rm -f core* *.stackdump
	rm -f *.exe
	rm -f Server1.o Server1.exe
	rm -f Server2.o Server2.exe
	rm -f Client1.o Client1.exe
	rm -f Client2.o Client2.exe
	rm -f Proxy.o Proxy.exe
	rm -f Util.o

cleanall:	clean
	rm -f *~

