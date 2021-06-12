#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<stdlib.h>
#include<time.h>
#include <stddef.h>
#include <stdbool.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/types.h>
#include <stddef.h>
#include <stdbool.h>
#define DEFAULT_PROTOCOL 0

//codice per strtok1 e is_delim preso da: https://gist.github.com/sushlala/3172c37f2947e96977b6a0682ec55a60

bool is_delim(char c, char *delim) //Funzione  per controllare se una stringa è delimitata da un determinato argomento
{
  while(*delim != '\0')
  {
    if(c == *delim)
      return true;
    delim++;
  }
  return false;
}

char *strtok1(char *s, char *delim) //sostituto di strtok
{
  static char *p;  
  if(!s)
  {
    s = p;
  }
  if(!s)
  {
    return NULL;
  }

  while(1)
  {
    if(is_delim(*s, delim))
    {
      s++;
      continue;
    }
    if(*s == '\0')
    {
      return NULL;
    }
    break;
  }

  char *ret = s;
  while(1)
  {
    if(*s == '\0')
    {
      p = s; 
      return ret;
    }
    if(is_delim(*s, delim))
    {
      *s = '\0';
      p = s + 1;
      return ret;
    }
    s++;
  }
}
int calcolastringa(char stringa[]){ /*Funzione per sommare i valori dei caratteri di una stringa partendo dalla fine*/
int cont=0;
int tot=0;
int lunghezza=strlen(stringa);
while(0<=lunghezza){
    tot+=stringa[lunghezza];
    lunghezza--;
}
return tot;
}



int secalg(char stringa[]){ //secondo algoritmo per il calcolo del valore della stringa
  int fin=0;
  char * pch;
  pch = strtok1(stringa,",");
  while (pch != NULL)
  {
    fin+=calcolastringa(pch);
 
    pch = strtok1(NULL, ",");
  }
return fin;
}

int calcolainvia(int fail){
	int clientFd, serverLen, result,ris;
	char buffer[1000]; //Qui scriverò il messaggio da inviare
	char * out = "filecond2";
 	struct sockaddr_un serverUNIXAddress;
 	struct sockaddr* serverSockAddrPtr;
 	serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
 	serverLen = sizeof (serverUNIXAddress);
 	clientFd = socket (AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
 	serverUNIXAddress.sun_family = AF_UNIX; /* Dominio del Server */
 	strcpy (serverUNIXAddress.sun_path, "mysocket");/*Nome del Server*/
 	do { /* Ciclo finchè non riesco  a connetermi ad un Server */
 	    result = connect (clientFd, serverSockAddrPtr, serverLen);
 	} while (result == -1);
 while(1){
 	int i=read(clientFd, buffer, 1000); //leggo dalla socket
	if(i!=0&i!=-1){  //controlo di aver letto una stringa e di non avere avuto errori
		if(fail==2&& (rand()%20)==0){  //se è attiva la modalità fallimento genero un numero casuale e ne faccio il modulo 20 in modo da avere una posibbiltà di errore di un ventesimo
  		ris=secalg(buffer)+20;
  		}else{
  		ris=secalg(buffer);
  		}
		int fdfn=open(out,O_WRONLY|O_CREAT|O_TRUNC,0777); //creo un file condiviso
		char * stringars;
		sprintf(stringars, "%d", ris);
		write(fdfn, stringars,strlen(stringars)+1); //scrivo il risultato nel file condiviso
		close(fdfn); //chiudo il file condiviso
 	}
 	}
}
int generafilepid(char nomefile[]){ //metodo che genera un file contenente il PID di questo processo
	int pid = getpid();
	int fd = open(nomefile,O_CREAT|O_WRONLY|O_TRUNC, 0777);
	char stringapid[10];
	sprintf(stringapid, "%d", pid);
	write(fd, stringapid,strlen(stringapid));
	close(fd);
	}
int main(int argc, char *argv[]){
srand(time(NULL)+21); //genero un seed casuale a cui aggiungo un altro numero per evitare che generi numeri identici a quelli degli altri processi
generafilepid("FILEPID/PIDp2");
calcolainvia(argc);
}
