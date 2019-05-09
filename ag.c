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
#define FIFO "/tmp/so"

struct venda
{
	int cod;
	int qtd;
	double montante;
};
typedef struct venda* mVenda;

int main(){

	char* linhaVenda = malloc(sizeof(char*)*MAX);
	mVenda* venda = malloc(sizeof(struct venda)*MAX);
	mVenda vendaAux = malloc(sizeof(struct venda));
	char* vendaAg = malloc(sizeof(char*)*MAX);
	int flag=0, i=1, j=0, k=0;
	time_t now;
	struct tm *mytime = localtime(&now); 

	read(0, linhaVenda, MAX);
	do{
		vendaAux->cod = atoi(strtok(linhaVenda, " "));
		vendaAux->qtd = atoi(strtok(NULL, " "));
		vendaAux->montante = atof(strtok(NULL, " "));

		flag = 0;
		for(j=0; j<(i-1); j++){
			if(venda[j] != NULL && vendaAux->cod == venda[j]->cod)
			{
				venda[j]->qtd += vendaAux->qtd;
				venda[j]->montante += venda[j]->montante;
				flag = 1;
				j=i-1;
			}
		i++;
		}
		if(flag == 0)
		{
			venda[i-1]->cod = vendaAux->cod;
			venda[i-1]->qtd = vendaAux->qtd;
			venda[i-1]->montante = vendaAux->montante; 
		}
	}while(read(0, linhaVenda, MAX) != 1);

	for (int k = 0; k < (i-1); k++)
	{
		snprintf(vendaAg, MAX, "%d %d %f", venda[k]->cod, venda[k]->qtd, venda[k]->montante);
		write(1,vendaAg, strlen(vendaAg));
	}

//snprintf(nomeFich, MAX, "%d-%d-%dT%d:%d:%d", mytime->tm_year+1900, mytime->tm_mon+1, mytime->tm_mday, mytime->tm_hour, mytime->tm_min, mytime->tm_sec);

//fAg = open(nomeFich, O_CREAT, O_WRONLY);	
}