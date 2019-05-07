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

int flag = 0;

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

ssize_t readln (int fd, void *buf, size_t nbyte) {
    nbyte--;
    char *cbuf = (char*) buf;
    int i;
    int rd=1;
    off_t foundnl = 0;

    rd = read (fd, cbuf, nbyte);

    for (i = 0; i < rd; i++)
        if (cbuf[i] == '\n'){
            foundnl = 1;
            break;
        }

    cbuf[i] = 0;

    lseek (fd, (i - rd) + foundnl, SEEK_CUR);
    return (foundnl == 1 ? i : -i);
}


int main()
{
	char buffer[20];
	char* primeiro = malloc(sizeof(char) * 10);
	char* segundo = malloc(sizeof(char) * 10);
	int argc;
	
	do
	{
		argc = 2;
		if(readln(0, buffer, 20) == -1)
			{perror("fim");exit(0);}

		strtok(buffer, "\n");

		primeiro = strtok(buffer, " ");

		if((segundo = strtok(NULL, " ")))
			argc ++;

		if(strlen(buffer))
		{
			switch(argc)
			{
				int qnt;
				case 0:
				case 1: perror("Poucos argumentos"); exit(-1);
				case 2: //mostra stock e preco
						qnt = show(primeiro);
						printf("%d\n",qnt);
						break;
				case 3: atualiza(primeiro, segundo);
						break;
				default: perror("Demasiados agurmentos"); exit(-1);
			}
		}
		else
			break;
	}while(1);

	return 0;
}