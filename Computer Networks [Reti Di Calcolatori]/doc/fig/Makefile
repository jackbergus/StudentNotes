SRC=$(wildcard *.dia)
DST=$(SRC:.dia=.pdf)

#compilazione dei file dia in pdf
%.pdf:		%.dia
			dia --filter=eps --export=$@_ $<	#generazione file .eps
			epstopdf $@_					#generazione file .pdf
			rm $@_						#rimozione file .eps

#Conversione dei grafici in DIA in PDF
dia2pdf:		$(DST)
