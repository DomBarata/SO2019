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

struct venda
{
	int cod;
	int qtd;
	double montante;
};
typedef struct venda* mVenda;

struct artigo
{
	int cod;
	double preco;
	int tamanhoStr;
};
typedef struct artigo* mArtigo;

struct stock
{
	int cod;
	int qtd;
};
typedef struct stock* mStock;

void show(int codigo, char* messageToClient) {
	
	int f, fArt;
	mStock stock = malloc(sizeof(struct stock));
	mArtigo artigo = malloc(sizeof(struct artigo));
	if((f = open("STOCKS", O_RDONLY)) == -1)
	{
		perror("open");
		exit(-1);
	}
	
	lseek(f, sizeof(struct stock) * (codigo-1), SEEK_CUR);
	read(f,stock,sizeof(struct stock));
	close(f);

	if((fArt = open("ARTIGOS", O_RDWR)) == -1)
	{
		perror("open");
		exit(-1);
	}

	lseek(fArt, (codigo-1)*sizeof(struct artigo), SEEK_SET);
	read(fArt, artigo, sizeof(struct artigo));
	close(fArt);

	snprintf(messageToClient, MAX, "Stock: %d -- Preço: %f", stock->qtd, artigo->preco);
}

void atualiza(int codigo, int quantidade, char* messageToClient)
{
	int f;
	int fVendas, fArt;
	char* venda = malloc(sizeof(char*)*MAX);
	mArtigo artigo = malloc(sizeof(struct artigo));
	mStock estrutura = malloc(sizeof(struct stock));
	
	if((f = open("STOCKS", O_RDWR)) == -1)
	{
		perror("open");
		exit(-1);
	}
	
	lseek(f, sizeof(struct stock) * (codigo-1), SEEK_SET);
	read(f,estrutura,sizeof(struct stock));

	if(quantidade >= (-estrutura->qtd))
	{
		estrutura->qtd += quantidade;

		lseek(f, sizeof(struct stock) * (codigo-1), SEEK_SET);
		write(f, estrutura, sizeof(struct stock));
		close(f);

		if(quantidade < 0)
		{
			if((fArt = open("ARTIGOS", O_RDWR)) == -1){
				perror("open");
				exit(-1);
			}
			lseek(fArt, sizeof(struct artigo)*(codigo-1), SEEK_SET);
			read(fArt, artigo, sizeof(struct artigo));
			close(fArt);
			
			snprintf(venda, MAX, "%d %d %f\n", codigo, -quantidade, (-quantidade)*(artigo->preco));
			
			if((fVendas = open("VENDAS", O_WRONLY)) == -1){
				perror("open");
				exit(-1);
			}
			lseek(fVendas,0,SEEK_END);
			write(fVendas, venda, strlen(venda));
			close(fVendas);
		}

		snprintf(messageToClient, MAX, "Novo stock do produto %d: %d", codigo, estrutura->qtd);
	}
	else
	{
		snprintf(messageToClient, MAX, "Stock insuficiente para venda!");
	}
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
		if(mkfifo(FIFO, 0666) != 0){
			perror("criacao fifo server");
			exit(-1);
		}
	}

	//signal(SIGINT,);

	

	while(1){
		//for (int i = 0; i < MAX; ++i)
		//{
		//	clientFIFO[i]  = 0;
		//	messageFromClient[i]  = 0;
		//	messageToClient[i]  = 0;
		//}
		memset(clientFIFO, 0, 100);
		memset(messageFromClient, 0, MAX);
		memset(messageToClient, 0, MAX);
		

		fromClient = open(FIFO, O_RDONLY,  O_NONBLOCK);
		while(!strlen(messageFromClient)){
			read(fromClient, messageFromClient,MAX);
		}
		close(fromClient);
		
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
			case 0:	show(codigo, messageToClient);
					break;
			case 1: quantidade = atoi(strtok(NULL,";"));
					atualiza(codigo, quantidade, messageToClient);
					break;
			case 2: 
			case 3: 
			default: perror("Demasiados argumentos"); exit(-1);
		}

		if((toClient = open(clientFIFO, O_WRONLY,  O_NONBLOCK)) == -1){
			perror("abertura fifo");
			exit(-1);
		}
		write(toClient, messageToClient, MAX);
		close(toClient);
	}

	return 0;
}