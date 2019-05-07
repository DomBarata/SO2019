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


struct stock
{
	int cod;
	int qtd;
};
typedef struct stock* mStock;

void show(int codigo, char* messageToClient) {
	
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

	snprintf(messageToClient, MAX, "Quantidade em stock: %d", estrutura->qtd);
}

void atualiza(int codigo, int quantidade, char* messageToClient)
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
	
	printf(" qunt %d\n", quantidade);				
	estrutura->qtd += quantidade;

	lseek(f, sizeof(struct stock) * (codigo-1), SEEK_SET);
	write(f, estrutura, sizeof(struct stock));
	close(f);

	snprintf(messageToClient, MAX, "Novo stock do produto %d: %d", estrutura->cod, estrutura->qtd);
}

int main()
{
	char* clientFIFO = malloc(sizeof(char*)*100);
	int option;
	int codigo;
	int quantidade;
	int res;
	int fromClient;
	int toClient;
	char messageFromClient[MAX];
	char messageToClient[MAX];
	
	if(access(FIFO, R_OK) == -1){
		if(mkfifo(FIFO, 0644) != 0){
			perror("criacao fifo server");
			exit(-1);
		}
	}

	while(1){
		fromClient = open(FIFO, O_RDONLY);
		read(fromClient, messageFromClient,MAX);
		close(fromClient);
		printf("ms %s\n",messageFromClient);
	
		clientFIFO = strtok(messageFromClient, ";");
		if(access(clientFIFO, W_OK) == -1)
		{
			perror("acesso fifo client");
			exit(-1);
		}

		option = atoi(strtok(NULL,";"));
		codigo = atoi(strtok(NULL,";"));
		switch(option)
		{
			case 0:
			case 1: perror("Poucos argumentos"); exit(-1);
			case 2: show(codigo, messageToClient);
					break;
			case 3: quantidade = atoi(strtok(NULL,";"));
					printf("x %d\n", quantidade );
					atualiza(codigo, quantidade, messageToClient);
					break;
			default: perror("Demasiados argumentos"); exit(-1);
		}

		if((toClient = open(clientFIFO, O_WRONLY)) == -1){
			perror("abertura fifo");
			exit(-1);
		}

		write(toClient, messageToClient, MAX);
		close(toClient);
	}

	return 0;
}