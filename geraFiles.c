#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[])
{
	char str[11] = "AAAAAAAAAA";;
	int fArt, fStr, fStk;
	mArtigo forWrite = malloc(sizeof(struct artigo));
	mStock varStock = malloc(sizeof(struct stock));


	forWrite->strPos = 0;

	fArt = open("ARTIGOS", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	fStr = open("STRINGS", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	fStk = open("STOCKS", O_WRONLY | O_CREAT | O_TRUNC, 0600);

	for(int i = 0; i < 500000; i++)
	{
		if (str[9] <= 'Z')
		{
			str[9]++;
		}
		else
		{
			str[9] = 'A';
			if (str[8] <= 'Z')
			{
				str[8]++;
			}
			else
			{
				str[8] = 'A';
				if (str[7] <= 'Z')
				{
					str[7]++;
				}
				else
				{
					str[7] = 'A';
					if (str[6] <= 'Z')
					{
						str[6]++;
					}
					else
					{
						str[6] = 'A';
						if (str[5] <= 'Z')
						{
							str[5]++;
						}
						else
						{
							str[5] = 'A';
							if (str[4] <= 'Z')
							{
								str[4]++;
							}
							else
							{
								str[4] = 'A';
								if (str[3] <= 'Z')
								{
									str[3]++;
								}
								else
								{
									str[3] = 'A';
									if (str[2] <= 'Z')
									{
										str[2]++;
									}
									else
									{
										str[2] = 'A';
										if (str[1] <= 'Z')
										{
											str[1]++;
										}
										else
										{
											str[1] = 'A';
											if (str[0] <= 'Z')
											{
												str[0]++;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		forWrite->cod = i + 1;
		forWrite->preco = i % 9 * 2 / 3;
		forWrite->tamanhoStr = strlen(str);
		
		varStock->cod = forWrite->cod;
		varStock->qtd = i % 5 * i/30 ;


		write(fArt, forWrite, sizeof(struct artigo));
		write(fStr, str, sizeof(str));
		write(fStk, varStock, sizeof(struct stock));

		forWrite->strPos += forWrite->tamanhoStr;
		printf("%d\n", i);
	}
	return 0;
}