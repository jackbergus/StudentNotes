

CC		= gcc
CFLAGS	= -ansi -std=c99 -Werror -Wall -pedantic
IDIR	= proxy/include/
INCLUDE	= -iquote $(IDIR)
CBASE 	= $(CC) $(CFLAGS) $(INCLUDE)
LFLAGS	= -lpthread

#	target directories 
MVBIN 	= ../
MVOBJ	= ../obj/
MVSRC	= proxy/
MVLIB	= ../lib/

#	hashtable library
LIB		= $(MVLIB)*.o

#	all proxy's parts 
PARTS	= $(MVOBJ)connect.o $(MVOBJ)communication.o $(MVOBJ)params.o $(MVOBJ)init.o $(MVOBJ)thread_functs.o $(MVOBJ)toserver.o $(MVOBJ)uni_list.o $(MVOBJ)toclient.o

#	general headers files
OTHER_H	= $(IDIR)bank_head.h $(IDIR)bank_functs.h $(IDIR)consts.h $(IDIR)init_globals.h $(IDIR)libhashtable.h $(IDIR)lists.h $(IDIR)macro.h $(IDIR)pthread_ext.h $(IDIR)types.h




.PHONY:	all	clean debug


#-------implicit .o rules-------------------------------------------
../obj/%.o: CFLAGS:= -c $(CFLAGS)
../obj/%.o: $(MVSRC)%.c $(IDIR)/%.h $(OTHER_H)
	$(CBASE) $< -o $@

#-------explicit rules-----------------------------------------
all:
	make ../proxy


debug:	CFLAGS:= -ggdb -DDEBUG $(CFLAGS)
debug:
	make all
	
		
../proxy: $(PARTS)
	$(CBASE) -o $@ $(LIB) $(PARTS) $(LFLAGS)
	

clean:
	rm -rf $(MVOBJ)*.o
	rm -rf ../proxy
	
	
	
	
