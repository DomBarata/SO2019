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
#define TAMSTR 32
#define FIFO "/tmp/so"

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

	printf("cod %d\n",artigo->cod);
	printf("preco %f\n",artigo->preco);
	printf("tam %d\n",artigo->tamanhoStr);

	return artigo->cod;
}

/*void testastrings(int codigo)
{
	int f, fd;
	mArtigo estrutura = malloc(sizeof(struct artigo));
	char string[TAMSTR];

	memset(string,0,TAMSTR);
 
	f = open("STRINGS", O_RDONLY);
	fd = open("ARTIGOS", O_RDONLY);

	lseek (fd, (codigo-1)*sizeof(struct artigo), SEEK_SET);

	read(fd,estrutura,sizeof(struct artigo));

	lseek (f, TAMSTR * (codigo-1), SEEK_SET);

	read(f,string,TAMSTR);
	
	printf("%s\n", string);
}*/

char* alteraNome(int codigo, char* nome){

	int fArt, fStr;
	mArtigo artigo = malloc(sizeof(struct artigo));

	if((fArt = open("ARTIGOS", O_RDWR)) == -1){
		perror("open");
		exit(-1);
	}
	lseek(fArt, sizeof(struct artigo)*(codigo-1), SEEK_SET);
	read(fArt, artigo, sizeof(struct artigo));

	artigo->tamanhoStr = strlen(nome);

	lseek(fArt, sizeof(struct artigo)*(codigo-1), SEEK_SET);
	write(fArt, artigo, sizeof(struct artigo));
	close(fArt);

	if((fStr = open("STRINGS", O_RDWR, O_APPEND)) == -1){
		perror("open");
		exit(-1);
	}
	lseek (fStr, TAMSTR * (codigo-1), SEEK_SET);
	write(fStr, nome, TAMSTR);
	close(fStr);

	//testastrings(codigo);

	return 0;
}

int alteraPreco(int codigo, double preco){

	int fArt;
	mArtigo artigo = malloc(sizeof(struct artigo));

	if((fArt = open("ARTIGOS", O_RDWR)) == -1)
	{
		perror("open");
		exit(-1);
	}

	lseek(fArt, (codigo-1)*sizeof(struct artigo), SEEK_SET);
	read(fArt, artigo, sizeof(struct artigo));

	artigo->preco = preco;

	lseek(fArt, (codigo-1)*sizeof(struct artigo), SEEK_SET);
	write(fArt,artigo,sizeof(struct artigo));
	close(fArt);

	//printf("%f\n"artigo->preco);

	return 0;
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
		case 'i': res = insere(segundo,atof(terceiro));
				  printf("Codigo do artigo: %d\n", res);
				  break;
		case 'n': alteraNome(atoi(segundo),terceiro);
				  break;
		case 'p': alteraPreco(atoi(segundo),atof(terceiro));
				  break;
		//case 's': testastrings(atoi(segundo));
		//		  break;
		default: perror("Demasiados argumentos"); exit(-1);
	}

}