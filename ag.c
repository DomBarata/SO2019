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
	int qtdVendas;
	int montante;
}
typedef struct venda* mVenda;

int main(){

	char* linhaVenda;
	mVenda* vendas;
	while(read(0, linhaVenda, MAX) != eof){
		venda[i]->cod = strtok(linhaVenda, " ");
		venda[i]->qtd = strtok(NULL, " ");
		venda[i]->montante = strtok(NULL, " ");
		i++;
	}

	nVendasAg = i;
	for(i=0; i<nVendasAg; i++){
		for(j=0; j<nVendasAg; j++){
			if(venda[i]->cod == venda[j]->cod){
				
			}
		}
	}

}