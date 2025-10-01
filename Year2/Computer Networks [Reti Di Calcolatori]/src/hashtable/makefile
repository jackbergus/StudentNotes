
CC		= gcc
CFLAGS	= -ansi -std=c99 -Werror -Wall -pedantic
IDIR	= include/
INCLUDE	= -iquote $(IDIR)
CBASE 	= $(CC) $(CFLAGS) $(INCLUDE)
LFLAGS	= -lpthread

#	target directories 
MVOBJ	= ../../lib/

#	all library's parts 
PARTS	= $(MVOBJ)filesystem.o $(MVOBJ)fsys.o $(MVOBJ)hash.o $(MVOBJ)hres.o $(MVOBJ)lett_scritt.o

#	general headers files
OTHER_H	= $(IDIR)consts.h



.PHONY:	all	clean debug

#-------implicit .o rules-------------------------------------------
../../lib/%.o:		CFLAGS:= -c $(CFLAGS)
../../lib/%.o: 		%.c $(IDIR)/%.h $(OTHER_H)
	$(CBASE) $< -o $@

all: $(PARTS)
	
	
clean:
	rm -rf $(MVOBJ)*.o	
	rm -rf test_hash
	
debug:	CFLAGS:= -ggdb -DDEBUG $(CFLAGS)
debug:	$(PARTS)
