#include <stdlib.h>
#include <stdio.h>
#include <string.h>	

unsigned char readDimensions(char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if(file == NULL)
	{
		perror("Erreur lors de la lecture du Sudoku d'entrée ");
		exit( EXIT_FAILURE);
	}
	
	int tmp;
	char buf[100];
	fgets(buf, 100, file);
	buf[strlen(buf)-1] = '\0'; //Pour supprimer le \n de fgets
	tmp = atoi(buf);
	if(tmp > 255)
	{
		printf("Erreur : Le programme ne peut gérer de sudoku dont la dimension dépasse 255\n");
		exit( EXIT_FAILURE );
	}
	fclose(file);
	
	return (unsigned char) tmp;
}
