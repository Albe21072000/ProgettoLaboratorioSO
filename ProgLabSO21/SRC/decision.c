#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
int prendinum(char* file){ //Metodo che restiuisce il numero contenuto in un file
	int fd=open(file, O_RDONLY);
	char buf[5];
	read(fd, buf, 5);
	close(fd);
	return atoi(buf);
	}
	
int ricevirisultatiedecidi(){
	char buffer[10], buffer1[10], buffer2[10];
	int fd3, fd1, fd2, ris1=0, ris3=0, ris2=0;
	char * out1 = "filecond1";;
     char * out3 = "filecond3";
     char * out2 = "filecond2";
     sleep(1);
	while(1){
		int pidWatch=prendinum("FILEPID/PIDwatch");
		kill(pidWatch, SIGUSR2); //segnalo al WatchDog che il processo è ancora vivo
		sleep(1); //aspetto un secondo che gli altri abbiano inviato i loro messaggi
		fd1=open(out1,O_RDONLY);  // per ogni processo di calcolo apro il file dove troverò il risultato e lo salvo in una variabile
		int i1=read(fd1, buffer1, 6);
		if(i1!=0&i1!=-1){
		ris1 =atoi(buffer1);
		remove(out1);
		}
		fd2=open(out2,O_RDONLY);
		int i2=read(fd2, buffer2, 6);
		if(i2!=0&i2!=-1){
		ris2 =atoi(buffer2);
		remove(out2);
		}
		fd3=open(out3,O_RDONLY);
		int i=read(fd3, buffer, 6);
		if(i!=0&i!=-1){
		ris3 =atoi(buffer);
		remove(out3);
		}
		int fd=open("LOG/Voted_Output",O_WRONLY|O_CREAT|O_TRUNC,0777); //apro e scrivo su un file nella cartella di LOG i risultati che ho ottenuto
		char stringarisultati[30];
		sprintf(stringarisultati, "P1:%d, P2:%d, P3:%d", ris1,ris2,ris3);
		write(fd, stringarisultati,strlen(stringarisultati)+1);
		close(fd);
		int fdrisultato=open("LOG/SYSTEM_LOG",O_WRONLY|O_CREAT|O_TRUNC,0777); //creo il file dove andrò a scrivere l'esito della decisione
		if(ris2==ris1||ris3==ris2||ris1==ris3){  //se è stato raggiunta una maggioranza scriverò lo scriverò nel file
		write(fdrisultato, "SUCCESSO",8);
		}
		else{ //altrimenti scrivo FALLIMENTO nel SYSTEM_LOG e manderò un segnale al failure manager
		write(fdrisultato, "FALLIMENTO",10);
		int pidFailManager=prendinum("FILEPID/PIDfail");
		kill(pidFailManager, SIGUSR1);
		}
		close(fd1); //chiudo tutti i file che ho usato
		close(fd3);
		close(fd2);
		close(fdrisultato);
	
	
  }
}
int generafilepid(char nomefile[]){  //metodo che genera un file contenente il PID di questo processo
	int pid = getpid();
	int fd = open(nomefile,O_CREAT|O_WRONLY|O_TRUNC, 0777);
	char stringapid[10];
	sprintf(stringapid, "%d", pid);
	write(fd, stringapid,strlen(stringapid));
	close(fd);
	}

int main(){
	umask(0000); //imposto il limitatore dei permessi a 0 per generare file che abbiano il massimo dei permessi
	generafilepid("FILEPID/PIDdec");
	ricevirisultatiedecidi();
}
