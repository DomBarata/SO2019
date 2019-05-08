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

int show(char* messageToServer, char* clientFIFO, char* code) 
{
	int codigo;
	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

	snprintf(messageToServer, strlen(clientFIFO) + 2*sizeof(char) + 2*sizeof(int), "%s;%d;%d", clientFIFO, 0, codigo);
	printf("%s\n", messageToServer);
	return 0;
}

int atualiza(char* messageToServer, char* clientFIFO, char* code, char* quant) 
{
	int codigo;
	int quantidade;
	
	quantidade = atoi(quant);
	printf("%d\n", quantidade );

	if((codigo = atoi(code)) < 0) 
	{
		perror("argumento nao inteiro"); 
		exit(-1);
	}

	snprintf(messageToServer, strlen(clientFIFO) + 3*sizeof(char) + 3*sizeof(int), "%s;%d;%d;%d", clientFIFO, 1, codigo, quantidade);
	return 0;
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
	char clientFIFO[100];
	char messageToServer[MAX];
	char messageFromServer[MAX];
	int toServer;
	int fromServer;
	int res;
	int argc = 0;
	char buffer[20];
	char* primeiro = malloc(sizeof(char) * 10);
	char* segundo = malloc(sizeof(char) * 10);
	
	while(1){
		argc = 0;
		
		if(readln(0, buffer, 20) == -1)
				{perror("fim");exit(0);}

		primeiro = strtok(buffer, " ");

		if((segundo = strtok(NULL, " "))){
			argc++;
		}
		
		if(!strlen(buffer)) break;

		if(access(FIFO, W_OK) == -1){
			perror("acesso escrita fifo server");
			exit(-1);
		}

		for (int i = 0; i < 100; ++i)
			clientFIFO[i]  = 0;

		snprintf(clientFIFO, sizeof("/tmp/so_") + sizeof(int), "/tmp/so_%d", getpid());
		if(access(clientFIFO, R_OK) == -1){
			if(mkfifo(clientFIFO, 0622) != 0){
				perror("criacao fifo client");
				exit(-1);
			}
		}

		switch(argc)
		{
			
			case 0:	res = show(messageToServer, clientFIFO, primeiro);
					break;
			case 1: res = atualiza(messageToServer, clientFIFO, primeiro, segundo);
					break;
			default: perror("Demasiados argumentos"); exit(-1);
		}

		if(!res)
		{
			toServer = open(FIFO, O_WRONLY);
			write(toServer,messageToServer,strlen(messageToServer));
			close(toServer);
			printf("msts %s\n", messageToServer);

			fromServer = open(clientFIFO, O_RDONLY);
			read(fromServer, messageFromServer, MAX);
			close(fromServer);
			printf("msfs %s\n", messageFromServer);

			write(1, messageFromServer, strlen(messageFromServer));
			write(1,"\n",1);
		}
	}
	return 0;
}