#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

void ucciditutti(){  /*quando ricevo il segnale termino tutti i processi*/
	unlink("mysocket"); /*Distruggo la socket e la pipe per evitare che in caso di fallimento essa possano generare problemi*/
	unlink("myfifo"); 
	system("killall p3&killall p2 &killall p1&killall dec&killall input& killall fail&killall watch");
	}
	int generafilepid(char nomefile[]){ /*Genero un file PID per poter terminare questo processo quando termino il file da leggere*/
	int pid = getpid();
	int fd = open(nomefile,O_CREAT|O_WRONLY|O_TRUNC, 0777); 
	char stringapid[10];
	sprintf(stringapid, "%d", pid);
	write(fd, stringapid,strlen(stringapid));
	close(fd);
	}
	
int main(void){
	signal(SIGUSR1, *ucciditutti); /*imposto l'esecuzione del metodo per uccidere i processi una volta ricevuto un segnale SIGUSR1*/ 
	generafilepid("FILEPID/PIDfail");
	while(1){
	sleep(1);
	}
	}
