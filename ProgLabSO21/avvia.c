#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
int main(int argc, char *argv[]){
	umask(0000);
	mkdir("LOG", 0777);  /*creo una cartella per i vari file di log*/
	mkdir("BIN", 0777); /*creo una cartella per i file compilati*/
	if(argc<3){ /*se sono stati passati meno di due argomenti termino con un errore critico*/
		printf("ERRORE CRITICO: INSERIRE FILEPATH E MODALITA' DI AVVIO!\n");
		int fdrisultato=open("LOG/SYSTEM_LOG",O_WRONLY|O_CREAT|O_TRUNC,0777);
		write(fdrisultato, "ERRORE CRITICO!",15);
		close(fdrisultato);
	}else{
		char comp[1000];
		if(strcmp(argv[1], "NORMALE")==0){
			
			sprintf(comp, "./BIN/p3&./BIN/input %s&./BIN/dec&./BIN/p1&./BIN/p2&./BIN/fail&./BIN/watch", argv[2]);
		}else if(strcmp(argv[1], "FALLIMENTO")==0){
			sprintf(comp, "./BIN/p3 1&./BIN/input %s&./BIN/dec&./BIN/p1 1&./BIN/p2 1&./BIN/fail&./BIN/watch", argv[2]); /*inserisco il filepath da visitare nella stringa che eseguirò*/
		}else{        /*Se la modalità di esecuzione passata non è valida termino con un errore critico*/
			printf("ERRORE CRITICO: MODALITA' DI AVVIO NON VALIDA!\n");
			int fdrisultato=open("LOG/SYSTEM_LOG",O_WRONLY|O_CREAT|O_TRUNC,0777);
			write(fdrisultato, "ERRORE CRITICO!",15);
			close(fdrisultato);
			exit(-1);
			}
		system("chmod 777 BIN");  /*imposto i permessi sulla directory BIN al massimo per poterla gestire senza problemi*/
		system("cc SRC/P3.c -o BIN/p3 &cc SRC/InputManager.c -o BIN/input&cc SRC/decision.c -o BIN/dec&cc SRC/P1.c -o BIN/p1&cc SRC/P2.c -o BIN/p2&cc SRC/Failure_Manager.c -o BIN/fail&cc SRC/Watchdog.c -o BIN/watch"); /*se il programma è avviato in modalità fallimento passo un argomento qualsiasi ai tre processi di calcolo*/
		mkdir("FILEPID", 0777);
		sleep(1);
		system("chmod 777 BIN/p2&chmod 777 BIN/p3&chmod 777 BIN/input&chmod 777 FILEPID&chmod 777 LOG"); /*imposto i permessi al massimo*/
		sleep(1);
		system(comp);
		}
}
