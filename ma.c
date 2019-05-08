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
	
	int fArt, fStr, fStk;
	mArtigo artigo = malloc(sizeof(struct artigo));
	mStock stock = malloc(sizeof(struct stock));

	if((fStr = open("STRINGS", O_WRONLY, O_APPEND)) == -1)
	{
		perror("open");
		exit(-1);
	}
	write(fStr,nome,strlen(nome));
	close(fStr);

	if((fArt = open("ARTIGOS", O_RDWR)) == -1)
	{
		perror("open");
		exit(-1);
	}

	lseek(fArt, -sizeof(struct artigo), SEEK_END);
	read(fArt, artigo, sizeof(struct artigo));

	artigo->cod++;
	artigo->preco = preco;
	artigo->strPos += artigo->tamanhoStr;
	artigo->tamanhoStr = strlen(nome);

	lseek(fArt,0,SEEK_END);
	write(fArt,artigo,sizeof(struct artigo));
	close(fArt);

	stock->cod = artigo->cod;
	stock->qtd = 1; 

	if((fStk = open("STOCKS", O_WRONLY, O_APPEND)) == 0)
	{
		perror("open");
		exit(-1);
	}
	write(fStk, stock, sizeof(struct stock));
	close(fStk);

	return artigo->cod;
}

char* alteraNome(int codigo, char* nome){

	int fArt, fStr;
	mArtigo artigo = malloc(sizeof(struct artigo));

	if((fArt = open("ARTIGOS", O_RDWR)) == -1){
		perror("open");
		exit(-1);
	}
	lseek(fArt, sizeof(struct artigo)*(codigo-1), SEEK_SET);
	read(fArt, artigo, sizeof(struct artigo));
	close(fArt);

	if((fStr = open("STRINGS", O_RDWR, O_APPEND)) == -1){
		perror("open");
		exit(-1);
	}
	lseek(fStr, artigo->strPos, sizeof(nome));
	write(fStr, nome, sizeof(nome));
	close(fStr);

	return 0;

}

int alteraPreco(){

}

void readLn(int file, char *buffer, int lim)
{
	fgets(buffer, lim, stdin);
}

int main(){

	int res;
	char* nome;
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
		case 'i': res = insere(segundo,atof(terceiro));
				  printf("Codigo do artigo: %d\n", res);
				  break;
		case 'n': alteraNome(atoi(segundo),terceiro);
				  break;
		case 'p':
				  break;
		default: perror("Demasiados argumentos"); exit(-1);
	}

}