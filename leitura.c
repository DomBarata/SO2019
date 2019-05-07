char buffer[20];
	char* primeiro = malloc(sizeof(char) * 10);
	char* segundo = malloc(sizeof(char) * 10);
	
	readLn(0, buffer, 20);
	strtok(buffer, "\n");
	printf("%s\n", buffer);

	primeiro = strtok(buffer, " ");
	printf("%s\n", primeiro);

	if((segundo = strtok(NULL, " ")))
		printf("%s %ld\n", segundo, strlen(segundo));


void readLn(int file, char *buffer, int lim)
{
	fgets(buffer, lim, stdin);
}