#include <stdlib.h>
#include <stdio.h>

#include "structures.h"


block** createGrid(unsigned char n, block*** gridAdress )
{
	int i;
	block** grid;
	
	if ( (grid = (block**) malloc( n*sizeof(block*))) == NULL)
	{
		perror("Problème de Malloc :");
		exit( EXIT_FAILURE );
	}
	
	for(i = 0 ; i < n ; i++)
	{
		if ((grid[i] = (block*) malloc( n*sizeof(block))) == NULL)
		{
			perror("Problème de Malloc :");
			exit( EXIT_FAILURE );
		}
	}
	
	*gridAdress = grid;//
	
	return grid;
}
////////////////////////////////////

void deleteGrid(unsigned char n, block** grid)
{
	int i;
	
	for(i = 0; i < n ; i++)
	{
		free(grid[i]);
	}
	free(grid);
	
	return;
}
