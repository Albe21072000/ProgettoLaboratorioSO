#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
	
int generafilepid(char nomefile[]){
	int pid = getpid();
	int fd = open(nomefile,O_CREAT|O_WRONLY|O_TRUNC, 0777);
	char stringapid[10];
	sprintf(stringapid, "%d", pid);
	write(fd, stringapid,strlen(stringapid));
	close(fd);
	}

	int prendinum(char* file){
		int fd=open(file, O_RDONLY);
		char buf[5];
		read(fd, buf, 5);
		close(fd);
		return atoi(buf);
		}
	void uccidi(int c){   /*Segnalo al Fail Manager di far terminare l'esecuzione*/
    	int pidFailManager=prendinum("FILEPID/PIDfail");
		kill(pidFailManager, SIGUSR1);
	}
	void reset(int c){ /*quando ricevo il segnale resetto il tempo di attesa richamando alarm per altri 2 secondi*/
		alarm(2);
	}
int main(void){
	generafilepid("FILEPID/PIDwatch");
	signal(SIGUSR2, reset);  // quando ricevo il segnale SIGUSR2 resetto il tempo di attesa
	signal(SIGALRM, uccidi); //quando ricevo il segnale SIGALRM da alarm segnalo il failmanager
	alarm(2);
	while(1){
	/*Attendo*/
	}
	}
