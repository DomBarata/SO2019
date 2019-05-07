#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 512
#define FIFO "/tmp/so"

int show(char* messageToServer, char* clientFIFO, char* code) 
{
	int codigo;
	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

	snprintf(messageToServer, MAX, "%s;%d;%d", clientFIFO, 2, codigo);
	return 0;
}

int atualiza(char* messageToServer, char* clientFIFO, char* code, char* quant) 
{
	int codigo;
	int quantidade;
	
	quantidade = atoi(quant);
	printf("%d\n", quantidade );

	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

	snprintf(messageToServer, MAX, "%s;%d;%d;%d", clientFIFO, 3, codigo, quantidade);
	return 0;
}

void readLn(int file, char *buffer, int lim)
{
	fgets(buffer, lim, stdin);
}

int main()
{
	char clientFIFO[100];
	char messageToServer[MAX];
	char messageFromServer[MAX];
	int toServer;
	int fromServer;
	int res;
	int argc = 2;

	char buffer[20];
	char* primeiro = malloc(sizeof(char) * 10);
	char* segundo = malloc(sizeof(char) * 10);
	

	readLn(0, buffer, 20);
	strtok(buffer, "\n");

	primeiro = strtok(buffer, " ");

	if((segundo = strtok(NULL, " "))){
		argc++;
	}
	
	if(access(FIFO, W_OK) == -1){
		perror("acesso escrita fifo server");
		exit(-1);
	}

	snprintf(clientFIFO, sizeof(clientFIFO), "/tmp/so_%d", getpid());
	if(access(clientFIFO,R_OK) == -1){
		if(mkfifo(clientFIFO, 0622) != 0){
			perror("criacao fifo client");
			exit(-1);
		}
	}


	switch(argc)
	{
		
		case 0:
		case 1: perror("Poucos argumentos"); exit(-1);
		case 2: res = show(messageToServer, clientFIFO, primeiro);
				break;
		case 3: res = atualiza(messageToServer, clientFIFO, primeiro, segundo);
				break;
		default: perror("Demasiados argumentos"); exit(-1);
	}

	if(!res)
	{
		toServer = open(FIFO, O_WRONLY);
		write(toServer,messageToServer,strlen(messageToServer));
		close(toServer);
		printf("msts %s\n", messageToServer);

		fromServer = open(clientFIFO, O_RDONLY);
		read(fromServer, messageFromServer, MAX);
		close(fromServer);
		printf("msfs %s\n", messageFromServer);

		write(1, messageFromServer, strlen(messageFromServer));
		write(1,"\n",1);
	}

	return 0;
}