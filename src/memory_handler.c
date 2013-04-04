#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "structures.h"
#include "solver.h"


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

void deleteSudoku(Sudoku* sudoku)
{
	int i;
	for(i = 0; i < sudoku->n ; i++)
	{
		free(sudoku->grid[i]);
	}
	free(sudoku->grid);
	free(sudoku);
	return;
}

//////////////////////////////////////

void initSudoku( Sudoku** sudokuAdress, unsigned char** grid, unsigned char npar)
{
	
	Sudoku* sudoku = malloc(sizeof(Sudoku));
	if(sudoku == NULL)
	{
		perror("Erreur de Mémoire (Malloc)");
		exit( EXIT_FAILURE );
	}
	*sudokuAdress = sudoku;
	
	//Calcul de emptyBlocks
	int i, j;
	sudoku->emptyBlocks = 0;
	sudoku->n = npar;
	sudoku->grid = grid;
	
	for(i = 0 ; i < sudoku->n ; i++)
	{
		for(j = 0 ; j < sudoku->n ; j++)
		{
			if(grid[i][j] == 0)
			{
				sudoku->emptyBlocks++;
			}
		}
	}
	return;
}


void launchThreads( pthread_t** threadsAdress, int n)
{
	int widthSubSquare;
	void* tab = NULL;//à Voir comment on gère les arguments

	widthSubSquare = (int) sqrt( n );

	(*threadsAdress) = malloc( n * sizeof( pthread_t ) );
	if(*threadsAdress == NULL)
	{
		perror("Erreur de Malloc ");
		exit( EXIT_FAILURE );
	}

	for( int i = 0 ; i < n ; i++ )
	{
		if(pthread_create( &((*threadsAdress)[i]) , NULL, threadStart , tab) != 0)
        {
                perror("Erreur dans pthread_create");
                exit( EXIT_FAILURE );
        }

	}
	for( int i = 0 ; i < n ; i++ )
	{
		if(pthread_join( (*threadsAdress)[i], NULL ) != 0)
        {
                perror("Erreur dans pthread_join");
                exit( EXIT_FAILURE );
        }
	}printf("test\n");
    return;
}





