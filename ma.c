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

struct artigo
{
	int cod;
	double preco;
	int tamanhoStr;
	int strPos;
};
typedef struct artigo* mArtigo;

struct stock
{
	int cod;
	int qtd;
};
typedef struct stock* mStock;

int insere(char* nome, double preco){
	
	int f, fStr, fStk;
	mArtigo estrutura = malloc(sizeof(struct artigo));
	mStock stock = malloc(sizeof(struct stock));

	if((fStr = open("STRINGS", O_RDONLY)) == -1)
	{
		perror("open");
		exit(-1);
	}
	lseek(fStr,0,SEEK_END);
	write(fStr,nome,strlen(nome));
	close(fStr);

	if((f = open("ARTIGOS", O_RDWR, O_APPEND)) == -1)
	{
		perror("open");
		exit(-1);
	}

	lseek(f, -(sizeof(struct artigo)), SEEK_END);
	read(f, estrutura, sizeof(struct artigo));

	estrutura->cod++;
	estrutura->preco = preco;
	estrutura->strPos += estrutura->tamanhoStr;
	estrutura->tamanhoStr = strlen(nome);

	lseek(f,0,SEEK_END);
	write(f,estrutura,sizeof(struct artigo));
	close(f);

	stock->cod = estrutura->cod;
	stock->qtd = 1; 

	if((fStk = open("STOCKS", O_WRONLY, O_APPEND)) == 0)
	{
		perror("open");
		exit(-1);
	}
	write(fStk, stock, sizeof(struct stock));
	close(fStk);

	return estrutura->cod;
}

int alteraNome(){

}

int alteraPreco(){

}

void readLn(int file, char *buffer, int lim)
{
	fgets(buffer, lim, stdin);
}

int main(){

	int res;
	char buffer[20];
	char* primeiro = malloc(sizeof(char) * 10);
	char* segundo = malloc(sizeof(char) * 10);
	char* terceiro = malloc(sizeof(char) * 10);

	readLn(0, buffer, 20);
	strtok(buffer, "\n");

	primeiro = strtok(buffer, " ");
	segundo = strtok(NULL, " ");
	terceiro = strtok(NULL, " ");

	switch(primeiro[0])
	{
		
		case 0: perror("Poucos argumentos"); exit(-1);
		case 'i': res = insere(segundo,atoi(terceiro));
				  printf("Codigo do artigo: %d\n", res);
				  break;
		case 'n':
				  break;
		case 'p':
				  break;
		default: perror("Demasiados argumentos"); exit(-1);
	}

}