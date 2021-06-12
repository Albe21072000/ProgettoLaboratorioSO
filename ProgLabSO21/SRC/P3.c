#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include<time.h>

//codice per strtok1 e is_delim preso da: https://gist.github.com/sushlala/3172c37f2947e96977b6a0682ec55a60

bool is_delim(char c, char *delim)
{
  while(*delim != '\0')
  {
    if(c == *delim)
      return true;
    delim++;
  }
  return false;
}

char *strtok1(char *s, char *delim)
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
int calcolastringa(char stringa[]){ /*Funzione per sommare i valori dei caratteri di una stringa partendo dall'inizio*/
int cont=0;
int tot=0;
int lunghezza=strlen(stringa);
while(cont<=lunghezza){
    tot+=stringa[cont];
    cont++;
}
return tot;
}

int teralg(char stringa[]){ //Terzo algoritmo per il calcolo del valore della stringa
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



int calcolainvia(char file[], int fail){
	char buffer[1000]; //Qui scriverò il messaggio ricevuto
	int fd;  //File descriptor del file con la stringa di input 
	int fdfn; //File descriptor del file in cui scrivere il risultato
	char stringars[10]; //Qui scriverò il messaggio da inviare
	int ris=0;
	char * out = "filecond3";
	while(1){
		fd=open(file,O_RDONLY);
		int i=read(fd, buffer, 1000);
		if(i!=0&i!=-1){           //controlo di aver letto una stringa e di non avere avuto errori
		if(fail==2&& (rand()%30)==0){
  		ris=teralg(buffer)+30;
  		}else{
  		ris=teralg(buffer);
  		}
		fdfn=open(out,O_WRONLY|O_CREAT|O_TRUNC,0777); //apro il file in cui scrivere il risultato
		sprintf(stringars, "%d", ris);  //con sprintf trasformo il risultato in una stringa
		write(fdfn, stringars,strlen(stringars)+1); //scrivo il risultato nel file condiviso
		close(fdfn);
		remove(file); //rimuovo il file condiviso da cui ho preso la stringa
		}
		close(fd); //comunque vada chiudo il file
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
	srand(time(NULL)+32); //genero un seed casuale a cui aggiungo un altro numero per evitare che generi numeri identici a quelli degli altri processi
	generafilepid("FILEPID/PIDp3");
	calcolainvia("FileCondiviso", argc);
}
