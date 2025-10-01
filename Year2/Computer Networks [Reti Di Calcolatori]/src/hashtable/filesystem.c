/*
 * filesystem.c
 * This file is part of hashtable
 *
 * Copyright (C) 2012 - Giacomo Bergami
 *
 * hashtable is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * hashtable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hashtable; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */


/* Questo programma effettua l'LS di tutto le sottodirectory e
   quella corrente */


#include "filesystem.h"



/**  recursiveDelete():
 *  Effettua l'eliminazione ricorsiva degli elementi all'interno della directory
 *  @param dirname:	Percorso della cartella da svuotare ricorsivamente
 */
void recursiveDelete(char* dirname) {

  char* tmp;
  DIR *direct;
  struct dirent *dir;

  char *abs_filename;
  asprintf(&tmp, "%s", dirname);
  
  while ((tmp[strlen(tmp)-1]=='/')&&(strlen(tmp)))
  	tmp[strlen(tmp)-1] = '\0';
  if (!strlen(tmp))
  	return;
   
  printf("%s",tmp); 

  direct = opendir (tmp);
  if (direct) {
      while ( (dir = readdir (direct)) !=NULL){
        struct stat stt;

        asprintf(&abs_filename, "%s/%s", tmp, dir->d_name);
        stat(abs_filename, &stt);
        

        if(S_ISDIR(stt.st_mode)) {
          if(strcmp(dir->d_name, ".") && 
             strcmp(dir->d_name, "..")) {
             printf("Folder %s....\n", abs_filename); 
            recursiveDelete(abs_filename);
          }
        } else {
        	  printf("Deleting %s....\n", abs_filename);
                  unlink(abs_filename);
        }
        
        free(abs_filename);
        
      }
      closedir(direct);
      remove(tmp);
  }
  
  free(tmp);

}


				/**************************************/
				/** Funzioni accessorie per la cache **/
				/**************************************/

/** file_exist():
 *  Verifica l'esistenza del file \a filename.
 *  Se il pathname esiste ed è un file, restituisce 1,
 *  se esso è una cartella restituisce 2, se non esiste 0. 
 *  @param filename:	file locale del quale controllare l'esistenza
 *  @param time:	Conterrà il tempo da Epoch della creazione del file
 *  @param is_blank:	Restituisce, se passato come argomento, se
 *                      il file è solamente troncato 
 *  @param retbuf:	Restituisce lo fstat del file, se esiste.
 *  @param pfd:		Possibile (o NULL) puntatore al FileDescriptor: in
 *			quel caso il file non verrà chiuso
 */
char file_exists(char * filename, unsigned int *time, char *is_blank, \
		struct stat *retbuf, int *pfd){
    
    int fd    = open(filename, O_RDWR);
    char test = (char)0;

    if ((filename)&&(fd>-1)) {
      
		struct stat buf;
	
		fstat(fd, &buf); 
		if (time)
		   *time = (unsigned int)buf.st_ctime;

		if (is_blank)
		   *is_blank = '\0';
				    
		if (S_ISDIR(buf.st_mode)) test=(char)2;
		else test=(char)1; 
				    
		if (!pfd) close(fd); else *pfd = fd;
		if (retbuf) *retbuf = buf;
	
		return test;   
    } 
    
    return (char)0;
}


char folder_empty(char *dirname) {
  int n = 0;
  DIR *dir = opendir(dirname);
  
  /* Se la cartella non esiste */
  if (!(dir = opendir(dirname)))
    return (char)-1;
  
  /* Fino a tre elementi */
  while (readdir(dir)) {
    if(++n > 2) 
      break;
  }
  closedir(dir);
  
  /* Se ci sono almeno due elementi, questi sono . e .. */
  return ((char)(n <= 2)); 
}

					/***************************/
					/** Funzioni fondamentali **/
					/***************************/

/** obtain_local():
 *  Dato il pathning remoto, alloca in una stringa l'eventuale pathning locale corrispondente 
 *  @param remote: percorso remoto del quale ottenere la conversione
 *                 alla stringa corrente
 */
char* obtain_local(char* remote) {

	char *prova, *surf;

	/* Copio in prova tutto remote tranne 'mhttp://' */
	asprintf(&prova,"cache/%s",remote+8);
	surf=prova;
	
	/* Finché non trovo un ':' e non supero la dimensione di prova */
	while ((*surf++ != ':')&&(surf<=prova+strlen(prova)*sizeof(char)));
	if (surf[-1]==':') surf[-1]='/';
	
	return prova;

}

/** resource_exists():
 *  Questa funzione verifica se il path remoto esista o meno. In base al valore 
 *  del parametro \a deallocate, viene restituita o meno la stringa convertita 
 *  della risorsa locale corrispondente
 *  @param remote: 	path remoto
 *  @param deallocate:  indica se deallocare l'indirizzo locale della risorsa 
 *                      corrispondente
 *  @param result:	verrà scritto in questo puntatore l'esistenza o meno del
 *                      file
 *  @param time:	tempo di creazione della risorsa
 *  @param blank:	restituisce se il file è stato troncato o meno (e quindi
 *                      vuoto)
 *  @param retbuf:	restituisce (se non NULL) il puntatore all'fstat del 
 *                      file in cache
 *  @param pfd:		Possibile (o NULL) puntatore al FileDescriptor: viene
 *			passato un fd aperto.
 */
char* 
resource_exists(char* remote, int deallocate, char* result, unsigned int* 	time,
															char* 			blank,
															struct stat*	retbuf, 
															int* 			pfd)
{
		
	char *prova = obtain_local(remote);
	*result = file_exists(prova, time, blank, retbuf, pfd);
	
	/* Se non mi interessa conoscere il percorso locale associato alla risorsa
	 * remota */
	if (deallocate)
	{
		free(prova);
		return NULL;
	}
	/* Altrimenti non viene deallocato */
	else 
		return prova;
}

/**  re_new_resource():
 *  Data il path della cache, crea la nuova risorsa: se le cartelle intermedie
 *  non esistono, esse vengono create, ed il file finale viene troncato ed aperto
 *  alla dimensione base massima. Restituisce la data di creazione del file
 *  @param cache_path: 	Identifica il percorso locale della risorsa
 *  @param retbuf:	Identifica lo stat del file che è stato aperto.
 *			Se non NULL viene aggiornato.
 *  @param pfd:		Identifica il FileDescriptor del nuovo file. Se non
 * 			NULL viene aggiornato, altrimenti viene chiuso.
 */
void 
re_new_resource(char* cache_path, struct stat *retbuf, int *pfd)
{
	unsigned int 	toret; 	
	struct stat		buf;
	
	char 	buffer[5000];
	char 	*surf, *tmp2;
	char 	tmp;
	int 	fd;

	
	toret	= 0;
	memset(buffer,0,5000);	
	
	
	asprintf( &surf, "%s", cache_path);
	tmp2 = surf;
	
	/* Cancellazione ricorsiva delle cartelle vuote */
	while ((( 		(ssize_t) tmp2 - (ssize_t) surf) / sizeof(char)) 
				<= 	strlen(cache_path)*sizeof(char)) 
	{
		if( *tmp2++ != '/');
		/* Se ho trovato una cartella nel percorso */
		else 
		{
			tmp = *tmp2;
			*tmp2 = '\0';
			
			/* Se non esiste, la creo */
			if( file_exists( surf, NULL, NULL, NULL, NULL) == 0)
				mkdir(surf,0777);
			
			*tmp2 = tmp;
		}
	}
	
	/* Alla fine creo il file */
	fd = open( cache_path, O_WRONLY | O_CREAT, 0777);
	/* Riservo una memoria fittizia */
	/*write(fd,buffer,5000);*/
	fstat( fd, &buf); 
	toret = buf.st_ctime;
	
	/* Se passo pfd a NULL, voglio chiudere il fd (non mi importa appunto
	 * conoscerne il valore*/
	if( !pfd) 
		close(fd); 
	/* altrimenti non chiudo il file, e salvo il valore del filedescriptor*/
	else 
		*pfd = fd;
	
	free( surf);
	
	/* Se mi interessa conoscere lo stat del file, allora lo copio */
	if( retbuf) 
		*retbuf = buf;
	
}


void 
resource_remove(char* path) 
{

	char tmp2;
	char *filecache;
	char *tmp;
	
	asprintf(&filecache,"%s",path);
	tmp = &filecache[strlen(filecache)-1];

	/* Rimuovo il file associato */
	remove(path);
	
	/* Finchè non arrivo alla fine della stringa */
	while (tmp>=filecache)
	{
		/* se risalendo il percorso ho trovato una cartella */
		if (*tmp--=='/') 
		{
			tmp2 = *(tmp+1);
			*(tmp+1)=(char)0;
			
			/* Se non ci sono altri files all'interno della cartella, allora
			 * posso rimuovere la cartella associata */
			if (folder_empty(filecache)) 
			{
				rmdir(filecache);
				*(tmp+1)=(char)0;
			}
			/* altrimenti, se già in questa cartella ci sono dei files, 
			 * conseguentemente non potrò eliminare le eventuali cartelle
			 * superiori */
			else 
			{
				free(filecache);
				return;
			}
		}
	}
	free(filecache);
	
}
