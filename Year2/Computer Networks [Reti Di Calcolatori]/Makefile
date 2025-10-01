
MVLIB = src/hashtable/
MVOTH = src/
MVGDE = doc/

.PHONY:	all lib	clean debug proxy guide

all:
	make lib && make proxy

lib:
	cd $(MVLIB) && make all 	
	
proxy:
	cd $(MVOTH) && make all

clean:
	cd $(MVLIB) && make clean
	cd $(MVOTH) && make clean
	
debug:
	cd $(MVLIB)	&& make debug
	cd $(MVOTH) && make debug

guide:	
	cd $(MVGDE) && make
	doxygen
	cd doxygen_latex && make && mv refman.pdf ../reference_manual.pdf
