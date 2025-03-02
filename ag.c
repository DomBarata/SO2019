#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX 512
#define BUFFERSIZE 50

struct venda
{
	int qtd;
	double montante;
};
typedef struct venda* Venda;

struct artigo
{
	int cod;
	double montante;
	int tamanhoStr;
};
typedef struct artigo* mArtigo;

ssize_t readln (int fd, void *buf, size_t nbyte) {
    nbyte--;
    char *cbuf = (char*) buf;
    int i;
    int rd = 1;
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

int main(){
	int f, i, aux;
	int codigo, quantidade;
	double montante;
	char buffer[BUFFERSIZE];
	char* strAux = malloc(sizeof(char) * 20);
	const time_t timer = time(NULL);
	Venda* arrayVendas;
	mArtigo artg = malloc(sizeof(struct artigo));

	if((f = open("ARTIGOS", O_RDONLY)) == -1)
	{
		perror("open artigos");
		exit(-1);
	}
 	lseek (f, -sizeof(struct artigo), SEEK_END);
 	read(f, artg, sizeof(struct artigo));
 	close(f);

 	arrayVendas = (Venda*)malloc(sizeof(struct venda*) * artg->cod);
 	for (i = 0; i < artg->cod; ++i)
 	{
 		arrayVendas[i] = malloc(sizeof(struct venda));
 		arrayVendas[i]->qtd = 0;
 		arrayVendas[i]->montante = 0;
 	}
	
	while(1){		
		readln (0, buffer, BUFFERSIZE);

		if(strlen(buffer) == 0) break;

		strAux = strtok(buffer, " ");
		codigo = atoi(strAux);
		strAux = strtok(NULL, " ");
		quantidade = atoi(strAux);
		strAux = strtok(NULL, " ");
		montante = atof(strAux);
		
		arrayVendas[codigo-1]->qtd += quantidade;
		arrayVendas[codigo-1]->montante += montante;
	}

	if((f = open(ctime(&timer), O_CREAT	| O_RDWR, 0666)) == -1)
		{
			perror("criar agregador");
			exit(-1);
		}

		i = 0;
		while(i < artg->cod)
		{
			if (arrayVendas[i]->qtd)
			{
				aux = i + 1;
				write(f, &aux, sizeof(int));
				write(f, arrayVendas[i], sizeof(struct venda));
			}
			i++;
		}
		close(f);
		free(artg);
		free(arrayVendas);
	
	return 0;	
}