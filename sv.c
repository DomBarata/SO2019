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


struct stock
{
	int cod;
	int qtd;
};
typedef struct stock* mStock;

int show(int codigo) {
	
	int f;
	mStock estrutura = malloc(sizeof(struct stock));

	if((f = open("STOCKS", O_RDONLY)) == -1)
	{
		perror("open");
		exit(-1);
	}
	
	lseek(f, sizeof(struct stock) * (codigo-1), SEEK_CUR);
	read(f,estrutura,sizeof(struct stock));
	close(f);

	return estrutura->qtd;
}

int atualiza(int codigo, int quantidade)
{
	int f;
	mStock estrutura = malloc(sizeof(struct stock));
	

	if((f = open("STOCKS", O_RDWR)) == -1)
	{
		perror("open");
		exit(-1);
	}
	
	lseek(f, sizeof(struct stock) * (codigo-1), SEEK_SET);
	read(f,estrutura,sizeof(struct stock));
	
	estrutura->qtd += quantidade;
	
	lseek(f, sizeof(struct stock) * (codigo-1), SEEK_SET);
	write(f, estrutura, sizeof(struct stock));
	close(f);

	return estrutura->qtd;
}

int main(int argc, char *argv[])
{
	char* clientFIFO = malloc(sizeof(char*));
	int option;
	int codigo;
	int quantidade;
	int res;
	int fromClient;
	int toClient;
	char* messageFromClient = malloc(sizeof(char*));
	char* messageToClient = malloc(sizeof(char*));
	
	if(access(FIFO, R_OK) == -1){
		if(mkfifo(FIFO, 0644) != 0){
			perror("criacao fifo");
			exit(-1);
		}
	}

	
	fromClient = open(FIFO, O_RDONLY, 0444);
	read(fromClient, messageFromClient, MAX);
	close(fromClient);

	clientFIFO = strtok(messageFromClient, ";");
	if(access(clientFIFO, W_OK) == -1)
	{
		perror("acesso fifo");
		exit(-1);
	}

	option = atoi(strtok(NULL,";"));
	switch(option)
	{
		case 0:
		case 1: perror("Poucos argumentos"); exit(-1);
		case 2: codigo = atoi(strtok(NULL,";"));
				res = show(codigo);
				snprintf(messageToClient, MAX, "Quantidade em stock: %d", res);
				break;
		case 3: codigo = atoi(strtok(NULL,";"));
				quantidade = atoi(strtok(NULL,";"));
				res = atualiza(codigo, quantidade);
				snprintf(messageToClient, MAX, "Novo stock: %d", res);
				break;
		default: perror("Demasiados argumentos"); exit(-1);
	}

	toClient = open(clientFIFO, O_WRONLY, 0444);
	write(toClient, messageToClient, strlen(messageToClient));
	close(toClient);

	return 0;
}