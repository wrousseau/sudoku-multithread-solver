#include "structures.h"


void** createGrid(unsigned char n, block*** gridAdress )
{
	int i, j;
	void** grid;
	
	if (grid = (block**) malloc( n*sizeof(block*)) == NULL)
	{
		perror("Problème de Malloc :");
		exit( EXIT_FAILURE );
	}
	
	for(i = 0 ; i < n ; i++)
	{
		if (grid[i] = (block*) malloc( n*sizeof(block)) == NULL)
		{
			perror("Problème de Malloc :");
			exit( EXIT_FAILURE );
		}
	}
	
	*gridAdress = grid;//
	
	return grid;
}
