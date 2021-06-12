#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include<time.h>

int calcolastringa(char stringa[]){ /*Funzione per sommare i valori dei caratteri di una stringa partendo dall'inizio*/
int cont=0;
int tot=0;
int lunghezza=strlen(stringa);
while(cont<lunghezza){      /*Finche non supero la lunghezza della stringa continuo a sommare*/
    tot+=stringa[cont];
    cont++;
}
return tot;
}

int primalg(char stringa[]){ /*primo algoritmo implementato con strtok*/
  int fin=0;
  char * pch;
  pch = strtok(stringa,",");
  while (pch != NULL)           /*Finchè strtok non restituisce NULL continuo a dividere e a calcolare il risultato*/
  {
    fin+=calcolastringa(pch);          
    pch = strtok(NULL, ",");
  }
return fin;
}
int leggicalcolainvia (int fail) {
	int fd1;
    char * myfifo = "myfifo";
  	int risfin;
    mkfifo(myfifo, 0666); //creo una  pipe con nome chiamata myfifo
  	int ris=0;
    char str1[1000];
    int fdfn=0;
    char stringars[10];      //Qui scriverò il messaggio da inviare
    char * out = "filecond1";
    while (1){	
        fd1 = open(myfifo,O_RDONLY); //apro la pipe in lettura e leggo il contenuto
        int e=read(fd1, str1, 1000);
        if(fail==2&& (rand()%10)==0){ //se è attiva la modalità fallimento genero un numero casuale e ne faccio il modulo 10 in modo da avere una posibbiltà di errore del 10%
  		ris=primalg(str1)+10;
  		}else{
  		ris=primalg(str1);
  		}
        close(fd1); //chiudo la pipe
        if(e>0){    //se ho letto almeno un  carattere scrivo il risultato su un file condiviso
        fdfn=open(out,O_WRONLY|O_CREAT|O_TRUNC,0777);
		sprintf(stringars, "%d", ris);
		write(fdfn, stringars,strlen(stringars)+1);
		}
		close(fdfn); //chiudo il file condiviso
    }
    return 0;
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
	srand(time(NULL));   //genero un nuovo seed per i numeri casuali
	generafilepid("FILEPID/PIDp1"); //genero il file PID per il processo
	leggicalcolainvia(argc);
}
