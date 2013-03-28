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








