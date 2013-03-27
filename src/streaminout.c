#include <stdlib.h>
#include <stdio.h>

unsigned char readDimensions(char* filePath)
{
	FILE* file = fopen (filePath, "r");
	if(file == NULL)
	{
		perror("Erreur lors de la lecture du sudoku d'entrée :");
		exit( EXIT_FAILURE );
	}
	
	int tmp;
	char buf[100];
	fgets( buf, 100, file);
	tmp = atoi(buf);
	if(tmp > 255)
	{
		perror("Erreur : la taille du sudoku dépasse la taille maximale autorisée par ce programme : 255.");
		exit( EXIT_FAILURE );
	}
	fclose(file);
	
	return (unsigned char) tmp;
}
