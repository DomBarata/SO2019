#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int ficheiros = atoi(argv[1]);
	int linhas = atoi(argv[2]);
	int prod, stock;
	FILE* f;
	char fileName[7];

	fileName[0] = 't';
	fileName[1] = 'e';
	fileName[2] = 's';
	fileName[3] = 't';
	fileName[4] = 'e';
	fileName[5] = 0;
	fileName[6] = 0;


	for (int i = 0; i < ficheiros; ++i)
	{
		fileName[5] = '1' + i;
		f = fopen(fileName, "w");
		for (int j = 0; j < linhas; ++j)
		{
			prod = rand()%499999+1;
			if (rand()%2)
			{
				stock = rand()%5000;
				fprintf(f, "%d %d\n", prod, stock);
			}
			else
			{
				fprintf(f, "%d\n", prod);
			}
		}
		fclose(f);
	}
}