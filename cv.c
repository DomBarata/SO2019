#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO "/tmp/so"

struct stock
{
	int cod;
	int qtd;
};
typedef struct stock* mStock;




int show(char* code) 
{
	int codigo;
	int f;

	mStock estrutura = malloc(sizeof(struct stock));

	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

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

void atualiza(char* code, char* quant) 
{
	int codigo;
	int quantidade;
	int f;
	mStock estrutura = malloc(sizeof(struct stock));
	
	quantidade = atoi(quant);
	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

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
}

int main(int argc, char *argv[])
{
	

	switch(argc)
	{
		int qnt;
		case 0:
		case 1: perror("Poucos argumentos"); exit(-1);
		case 2: //mostra stock e preco
				qnt = show(argv[1]);
				printf("%d\n",qnt);
				break;
		case 3: atualiza(argv[1], argv[2]);
				break;
		default: perror("Demasiados agurmentos"); exit(-1);
	}
	return 0;
}