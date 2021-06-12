#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/types.h>
#include <stddef.h>
#include <stdbool.h>
#define DEFAULT_PROTOCOL 0


int leggirigaeinvia(char nomefile[]){  
	/* inzializzo le variabili che mi serviranno per comunicare con i processi*/
	FILE* filePointer;
	filePointer = fopen(nomefile, "r"); /*apro in sola lettura il file da cui prendere le righe*/
	int bufferLength = 1000;
	char buffer[bufferLength]; /*buffer dove scrivo la stringa ottenuta*/
	int fd3; 
	int cont=0;
	int i;
	int pipefd;
	char * myfifo = "myfifo";
	mkfifo(myfifo, 0666); /*creo una pipe con nome chiamata myfifo"*/
	/*creo la socket lato Server*/
	int serverFd, clientFd, serverLen, clientLen;
    struct sockaddr_un serverUNIXAddress; /* Indirizzo Server*/
    struct sockaddr* serverSockAddrPtr; /*Puntatore alla struttura dellìindirizzo del server*/
    struct sockaddr_un clientUNIXAddress; /*Indirizzo del Client*/
    struct sockaddr* clientSockAddrPtr;
    serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
    serverLen = sizeof (serverUNIXAddress);
    clientSockAddrPtr = (struct sockaddr*) &clientUNIXAddress;
    clientLen = sizeof (clientUNIXAddress);
    serverFd = socket (AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
    serverUNIXAddress.sun_family = AF_UNIX; 
    strcpy (serverUNIXAddress.sun_path, "mysocket"); /*Imposto il nome */
    unlink ("mysocket"); /* Distruggo la precedente socket */
    bind (serverFd, serverSockAddrPtr, serverLen);/*Creo la socket*/
    listen (serverFd, 1); /* Imposto ad 1 il numero massimo di richieste pendenti*/
    clientFd = accept (serverFd, clientSockAddrPtr, &clientLen);
	while(fgets(buffer, bufferLength, filePointer)) { /*Prendo una riga alla volta finche non eusarisco il file*/
        if (cont>2){ /*Siccome fgets divide la stringa iniziale in 2 devo scartare i primi due valori ottenuti da essa*/
        	sleep(1); /*aspetta 1 secondo*/
        	write(clientFd, buffer, strlen(buffer)); /*scrivo la stringa su tutti i mie canali di comunicazione*/
			fd3=open("FileCondiviso",O_CREAT|O_WRONLY|O_TRUNC, 0777);
			i= write(fd3, buffer, strlen(buffer));
			close(fd3);
			pipefd = open(myfifo, O_WRONLY);
			write(pipefd, buffer, strlen(buffer)); 
			close(pipefd); /* chiudo la pipe ed il file condiviso*/
        }
    cont++;
	}
	fclose(filePointer);
}

int prendinum(char* file){ //Metodo che restiuisce il numero contenuto in un file
	int fd=open(file, O_RDONLY);
	char buf[5];
	read(fd, buf, 5);
	close(fd);
	return atoi(buf); // con atoi trasformo la stringa ottenuta in un numero
}

int generafilepid(char nomefile[]){  //metodo che genera un file contenente il PID di questo processo
	int pid = getpid();
	int fd = open(nomefile,O_CREAT|O_WRONLY|O_TRUNC, 0777);
	char stringapid[10];
	sprintf(stringapid, "%d", pid);
	write(fd, stringapid,strlen(stringapid));
	close(fd);
}
	
int main(int argc, char *argv[]){
	generafilepid("FILEPID/pdinp");
	leggirigaeinvia(argv[1]);	
	int pid3=prendinum("FILEPID/PIDp3"); /*prendo il PID di tutti i processi da terminare*/
	int piddec=prendinum("FILEPID/PIDdec");
	int pid1=prendinum("FILEPID/PIDp1");
	int pid2=prendinum("FILEPID/PIDp2");
	int pidfail=prendinum("FILEPID/PIDfail");
	int pidwatch=prendinum("FILEPID/PIDwatch");
	sleep(2); /* aspetto che tutti termino il loro lavoro prima di ucciderli*/
	kill(pid3, SIGKILL); /*termino tutti i processi*/
	kill(piddec, SIGKILL);
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);
	kill(pidfail, SIGKILL);
	kill(pidwatch, SIGKILL);
	unlink("mysocket"); /*Elimino la socket e la pipe*/
	unlink("myfifo");
}
