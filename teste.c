#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	printf("int %ld\n", sizeof(int));
	printf("float %ld\n", sizeof(float));
	printf("double %ld\n", sizeof(double));
	printf("char %ld\n", sizeof(char));
	if (argc == 2)
	{
		printf("%d\n", atoi(argv[1])%5);
	}

	return 0;
}