#include <stdlib.h>
#include <stdio.h>

#include "structures.h"


unsigned char** createGrid(unsigned char n, unsigned char*** gridAdress)
{
	int i;
	unsigned char** grid;
	
	if ( ( grid = (unsigned char**) malloc(n*sizeof(unsigned char*))) == NULL )
	{
		perror("Erreur de Mémoire (Malloc)");
		exit( EXIT_FAILURE );
	}
	
	for(i = 0 ; i < n ; i++)
	{
		if ( ( grid[i] = (unsigned char*) malloc(n*sizeof(unsigned char))) == NULL )
		{
			perror("Erreur de Mémoire (Malloc)");
			exit( EXIT_FAILURE );
		}
	}
	
	*gridAdress = grid;
	
	return grid;
}
////////////////////////////////////

void deleteGrid(unsigned char n, unsigned char** grid)
{
	int i;
	
	for(i = 0; i < n ; i++)
	{
		free(grid[i]);
	}
	free(grid);
	
	return;
}
