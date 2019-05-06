#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 4096
#define FIFO "/tmp/so"

int show(char* messageToSend, char* clientFIFO, char* code) 
{
	int codigo;
	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

	snprintf(messageToSend, MAX, "%s;%d;%d", clientFIFO, 2, codigo);
	return 1;
}

int atualiza(char* messageToSend, char* clientFIFO, char* code, char* quant) 
{
	int codigo;
	int quantidade;
	
	if((quantidade = atoi(quant)) < 0)
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

	snprintf(messageToSend, MAX, "%s;%d;%d;%d", clientFIFO, 3, codigo, quantidade);
	return 1;
}

int main(int argc, char *argv[])
{
	char clientFIFO[100];
	char* messageToServer = malloc(sizeof(char*));
	char* messageFromServer = malloc(sizeof(char*));
	int toServer;
	int fromServer;
	int res;
	
	if(access(FIFO, W_OK) == -1){
		perror("acesso escrita fifo");
		exit(-1);
	}

	snprintf(clientFIFO, sizeof(clientFIFO), "/tmp/so_%d", getpid());
	if(access(clientFIFO,R_OK) == -1){
		if(mkfifo(clientFIFO, 0622) != 0){
			perror("criacao fifo");
			exit(-1);
		}
	}

	switch(argc)
	{
		
		case 0:
		case 1: perror("Poucos argumentos"); exit(-1);
		case 2: res = show(messageToServer, clientFIFO, argv[1]);
				break;
		case 3: res = atualiza(messageToServer, clientFIFO, argv[1], argv[2]);
				break;
		default: perror("Demasiados argumentos"); exit(-1);
	}

	if(res)
	{
		toServer = open(FIFO, O_WRONLY, 0222);
		write(toServer,messageToServer,strlen(messageToServer));
		close(toServer);


		fromServer = open(clientFIFO, O_RDONLY, 0444);
		read(fromServer, messageFromServer, strlen(messageFromServer));
		close(fromServer);

		write(1, messageFromServer, strlen(messageFromServer));
	}
	return 0;
}