/*
 * communication.c
 * This file is part of proxy
 *
 * Copyright (C) 2012 - Giacomo Bergami, Paolo de Luca, Elena Fabbri
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
 * along with proxy. If not, see <http://www.gnu.org/licenses/>.
 */


#include "communication.h"

extern int oldoutptfd; /*TODO serve ancora ?? */

/******************************************************************************/
/* 		Funzioni primarie (di utilità al modulo)                      */
/* 	     Non sono quindi visibili esternamente ad esso                    */
/******************************************************************************/


/**
 *  haspos()
 * Dato un campo di testo \a text dove deve essere cercato \a BASE dalla 
 * posizione \a pos in \a text, restituisce -1 se BASE è inesistente dal primo
 * suo carattere, altrimenti la posizione successiva all'ultimo carattere in BASE.
 */
int haspos(char* text, char* BASE, int pos) {
	int i;
	for (i=0;i<strlen(BASE); i++)
		if ((!(text[pos+i]==BASE[i]))&&(!(BASE[i]=='\0'))) return -1;
	return pos+i;
}



/**
 *  surfto()
 * Dato un campo di testo \a text, si vuole cercare dal suo inizio il carattere 
 * base: verrà restituita la posizione successiva al ritrovamento del carattere,
 * altrimenti NULL
 */
char* 
surfto(char* text, char base, int text_size)
{
	int i;
	for (i=0; i<text_size; i++)
		if (text[i]==base) return &text[i+1];
	return NULL;
}



/**
 *  atreturns() 
 * 
 * FUNZIONE DI PARSING/LEXING
 * --------------------------
 * Restituisce eventualmente il puntatore alla stringa dopo due andate a capo 
 */
char* atreturns(char* buf,int len) {
	int  end=0;
	for (end=0; end<len; end++)
		if ((end>0)&&(buf[end]==buf[end-1])&&(buf[end]=='\n')) return &buf[end+1];
	return NULL;
}

/**
 *  atreturn() 
 * 
 * FUNZIONE DI PARSING/LEXING
 * --------------------------
 * Restituisce eventualmente il puntatore alla stringa dopo una andata a capo
 */
static char* atreturn(char* buf,int len) {
	int  end=0;
	for (end=0; end<len; end++)
		if (buf[end]=='\n') return &buf[end+1];
	return NULL;
}



/******************************************************************************/
/*     Funzione per la creazione delle risposte come stringhe restituibili    */
/******************************************************************************/

/**
 *   dorequest()
 * Funzione che stampa all'interno di un buffer di caratteri la richiesta da inoltrare
 * Restituisce 0 se la generazione è andata a buon fine, altrimenti 1 (se TYPE
 * non è un valore previsto da const.h)
 * @param request:	Percorso dove salvare il valore della richiesta
 * @param TYPE:		Tipo della richiesta da generare
 * @param path:		Percorso remoto della risorsa.
 * @param low:		Low Range della risorsa ottenuta
 * @param hight:	Hight Range della risorsa ottenuta. Se il valore è inferiore a low, allora inserisce solo il low.
 */
int dorequest(char* request, int TYPE, char* path, int low, int hight) {
	switch (TYPE) {
		case GET: 	{ if ((!low)&&(!hight))
					sprintf(request, "GET %s\n\n", path);
				  else if (low>hight)
				  	sprintf(request, "GET %s\nRange %d-\n\n", path, low);
				  else 
				  	sprintf(request, "GET %s\nRange %d-%d\n\n", path, low, hight);
				  return 0;		
				} break;
		case INF:	{
				  sprintf(request, "INF %s\n\n", path);
				  return 0;
				} break;
		default:	return 1;
	}
}

/**
 *  doreply()
 * Funzione che stampa all'interno di un buffer di caratteri la risposta da inoltrare.
 * Ritorna 0 se TYPE è un valore compreso tra le costanti predefinite.
 * @param reply:	Buffer dove viene stampato il risultato
 * @param TYPE:		Tipologia della richeista da inoltrare (const.h)
 * @param len:		Lunghezza della risposta che si sta ottenendo con la funzione
 * @param empire:	Expiry time
 * @param low:		Eventuale low range
 * @param hight:	Eventuale hight range. Se hight<low, allora non viene inserito l'upper
 * @param content:	Eventuale contenuto del messaggio
 */
int doreply(char *reply, int TYPE, int len, int empire, int low, int hight, \
	char* content) {
	switch (TYPE) {
		case OK:
				{
				  if (!content)
				  sprintf(reply, "%d\nLen %d\nExpire %d\n\n",\
				  	TYPE,len,empire);
				  else
				  sprintf(reply, "%d\nLen %d\nExpire %d\n\n%s",\
				  	TYPE,len,empire,content);
				  return 0;
				}
		case OK_RANGE:   {
		 		  if (!content)
		 		  sprintf(reply, "%d\nRange %d-%d\nExpire %d\n\n",\
				  	TYPE,low,hight,empire);
				  else
				  sprintf(reply, "%d\nRange %d-%d\nExpire %d\n\n%s",\
				  	TYPE,low,hight,empire,content);
				  return 0;
				}
	
		case NOT_FOUND:
		case INTERVAL_NOT_FOUND:
		case WRONG_REQUEST:
		case UNKNOWN_ERROR:
				{
				  sprintf(reply, "%d\n\n", TYPE);
				  return 0;
				} break;
		case INFO:
				{
				  sprintf(reply, "%d\nLen %d\nExpire %d\n\n",\
				  	TYPE,len,empire);
				  return 0;
				} break;
		default:	return 1;
	}

}





/******************************************************************************/
/* 	     Funzioni di parsing/lexing della risposta miniHTTP               */
/******************************************************************************/



/**
 *  parseHead()
 * Effettua il parsing della stringa all'interno di buf. 
 * Viene restituito il numero della risposta se è una risposta ben formata, 
 * altrimenti 0 (anche se buf è NULL).
 * Vengono passti per parametri i valori assunti dallo header del messaggio.
 * @param buf:		Eventuale buffer dal quale leggere la risposta ricevuta
 * @param buflen:	Lunghezza della parte di memoria effettiva in memoria
 * @param len:		Lunghezza del parametro LEN della richiesta INF
 * @param expire:	Expire time fornito dal messaggio dalla richiesta inf
 * @param low:		Low range
 * @param hight:	Hight Range
 * @param cont:		Punto dal quale continuare la lettura (puntatore al body)
 */
int parseHead(char* buf, int buflen, int* len, unsigned int* expire, int* low, int* hight, char** cont) {

	int n, resno, len_, low_, hight_;
	unsigned int 	expire_ = 0;
	n = resno = len_  = low_ = hight_ = 0;
	
	if (!buf) return 0;	
	
	/*resource *tmp; */
	n = sscanf(buf,"%d\n\n", &resno);
	
	if ((n)&&((resno==404)||(resno==405)||(resno==403)||(resno==402))) {
		return resno;
	} else {
	
		/* Gestione dei casi con argomenti Len ed Expire */
		n = sscanf(buf, "%d\nLen %d\nExpire %u\n\n*", &resno, &len_, &expire_);
		if ((n==3)&&((resno==202)||(resno==200))) {
		
			if ((cont)&&(resno!=202)) 
				*cont = atreturns(buf,buflen);
			
		} else {
	
			/* Ultimo caso rimasto: formato 201 */
			n = sscanf(buf, "201\nRange %d-%d\nExpire %u\n\n*", &low_, &hight_, &expire_);
			if (((n==2)&&(expire_)&&(!hight_)&&(low))||(n==3)) {

				if ((cont)) *cont = atreturns(buf,buflen);		
				resno = 201;
				
			} else
				/* La risposta non è ben formata */
			 	return 0;		
		}
	
	}
	
	
	
	if (len)	*len = len_;
	if (expire) 	*expire = expire_;
	if (low)	*low = low_;
	if (hight)	*hight = hight_;
	return resno;
}



/**
 *  parseRequest()
 * Funzione che effettua il parsing della richiesta remota.
 * @param buf:	Buffer contenente la risorsa 
 * @param len:	Lunghezza della risorsa del buffer 
 */
request* parseRequest(char* buf, int len) {
	int 	n, i, low, hight;
	char 	*k, *tmp;
	request* toret = malloc(sizeof(request));
	
	tmp=&buf[4];
	
	i=0;
	if ((buf[0]=='G')&&(buf[1]=='E')&&(buf[2]=='T')) {
		toret->TYPE=GET;
		k=atreturn(buf,len);
		kassert(k , "ERROR: couldn't find \\n");
		
		while (!(&k[-1]==tmp)) {
			toret->PATH[i++]=*tmp++;
		}
		toret->PATH[i]='\0';
		
		if (!getPort(getIP(toret->PATH, NULL), NULL)) {
			free(toret);
			return NULL;
		}
		
		n = sscanf(k, "Range %d-\n\n*", &low);
		if (n==1) {
			toret->rangelow=low;
			return toret;
		}
		
		n = sscanf(k, "Range %d-%d\n\n*", &low, &hight);
		if (n==2) {
			toret->rangelow=low;
			toret->rangehight=hight;
			return toret;
		}
		return toret;
		
	} else if ((buf[0]=='I')&&(buf[1]=='N')&&(buf[2]=='F')) {
		toret->TYPE=INF;
		k=atreturns(buf,len);
		
		kassert(k, "ERROR: couldn't find \\n\\n");
		while (!(&k[-2]==tmp)) {
			toret->PATH[i++]=*tmp++;
		}
		toret->PATH[i]='\0';
		
		if (!getPort(getIP(toret->PATH, NULL), NULL)) {
			free(toret);
			return NULL;
		}
		
	} else {
		free(toret);
		kassert(0,"Method not valid");
	}
		
	return toret;
	
}



/******************************************************************************/
/* 			Funzioni di gestione degli URL			      */
/******************************************************************************/

/**
 *   getIP()
 *  Questa funzione restituisce in dest il valore dell'IP fornito: restituisce
 *  NULL se il matching è stato scorretto, altrimenti il punto successivo da cui
 *  riprendere l'analisi della stringa 
 *  @param URL:		Contiene l'URL partendo da "mhttp://"
 *  @param dest:	Bufffer di destinazione dove salvare la stringa remota
 */
char* getIP(char* URL, char* dest) {

	char *start;
	char head[]="mhttp://";
	int i;
	
	start=URL;
	
	/* Controllo se la stringa contiene la head */
	for (i=0; i<strlen(head); i++)
		if (!(URL[i]==head[i])) return NULL;
		
	/* KPRINTF("Got Ip head (%d # %c)...\n", i, URL[i]); */
		
	
	/* Arrivo fino ai duepunti */
	while ((!(URL[i]==':'))&&(i<strlen(URL))) {
		/*putchar((int)'.');*/
		if (dest) *dest++=URL[i++]; else i++;
	}
	/*printf("NEXTCHAR: (%d # %c)\n", i, URL[i]);*/
	
	if (dest) *dest='\0';
	/*KPRINTF("%s\n", startdest);*/
	
	/* Controllo in che condizioni è arrivato alla fine */
	if (i>=strlen(start)) 
		return NULL;
	else
		return &URL[i];
}

/**
 *  getPort()
 * Questa funzione ottiene la porta del nostro URL, concordemente all'analisi
 * effettuata precedentemente per trovare l'IP. Si comporta in modo analogo a
 * getIP(): il puntatore restituito in caso di procedura corretta è il pathening
 */
char* getPort(char* URLp, char* dest) {
	
	char *start;
	int i=0;
	start=URLp;
	
	
	/*KPRINTF("Test2...\n");*/
	/* Continuo l'analisi solamente se quella precedente è corretta */
	if ((!URLp)||(!(*URLp==':'))) return NULL;
	i++;
	
	/*KPRINTF("Test2 Ok...\n");*/
	
	while ((i<strlen(start))&&(URLp[i]!='/'))
		if (dest) *dest++=URLp[i++]; else i++;
	
	/*KPRINTF("Exited While...\n");*/
	if (dest) *dest='\0';
	
	/* Controllo in che condizioni è arrivato alla fine */
	if (i>=strlen(start)) 
		return NULL;
	else
		return &URLp[i];
	
}


/******************************************************************************/
/* 			Funzioni di gestione degl mHTML			      */
/******************************************************************************/

/**
 *  getfirstlevel_next()
 * Questa funzione, dato un campo di test \a text ed un buffer \a buf di 
 * dimensioni \a len, se trova un qualsiasi tipo di risorsa memorizza il valore
 * ottenuto all'interno del suddetto buffer, altrimenti restituisce NULL
 * @param text:		Buffer di input
 * @param buf:		BUffer di output
 * @param len:		Lunghezza del buffer di output
 * @param text_len:	Lunghezza del buffer di input
 */
char* getfirstlevel_next(char* text, char* buf, int len, int text_len) {
	char *next;
	int  pos,i;
	
	i=0;
	/* Trovo la prima occorrenza di '<' */
	if ((!(next=surfto(text,'<',text_len)))&&\
		(((unsigned int)(next-text))/sizeof(char)>text_len)) return NULL;
	
	/* Trovo l'inizio di 'R' all'interno del tag */
	if ((!(next=surfto(next,'R',text_len)))&&\
		(((unsigned int)(next-text))/sizeof(char)>text_len)) return NULL;
	
	/* Trovo la coincidenza con <...REF= */
	if (((pos=haspos(next,"EF=",0))==-1)&&\
		(((unsigned int)(next-text))/sizeof(char)>text_len)) return NULL;
	
	/* Trovo la risorsa, che termina con il carattere '>' */
	while ((!(next[pos]=='>'))&&(i<len)) {
		if ((i<len)&&((next-text)/sizeof(char)+1>text_len)) return NULL;
		buf[i++]=next[pos++];
	}
	buf[i]='\0';
	return &next[pos+1];
}

/** get_resource_alloc():
 *  Restituisce una stringa ove è allocata la risorsa indicato dal buffer
 *  passato come argomento
 *  @param text:	puntatore al buffer di memoria dove è contenuta la risposta
 *  @param next:	puntatore ad un puntatore a carattere che verrà aggiornato
 *                      con la posizione dalla quale riprendere la scansione con
 *                      la get_resource_alloc 
 */
char* get_resource_alloc(char* text, char** next) {
	char buf[MAX_PATH];
	char* tonext = getfirstlevel_next(text,buf,MAX_PATH,strlen(text));
	char* toret;
	int len = strlen(buf);
	
	if (tonext==NULL) return NULL;
	toret =(char*)malloc(len+1);
	strcpy(toret,buf);
	toret[len]='\0';
	if (next) *next = tonext;
	return toret;
}
