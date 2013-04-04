#include <stdlib.h>
#include <stdio.h>

#include "structures.h"


void createGrid ( unsigned char n , unsigned char*** gridAdress )
{
	if ( ( *gridAdress = ( unsigned char** ) malloc( n*sizeof( unsigned char* ) ) ) == NULL )
	{
		perror( "Erreur de Mémoire (Malloc)" );
		exit( EXIT_FAILURE );
	}
	
	for( int i = 0 ; i < n ; i++)
	{
		if ( ( (*gridAdress)[i] = ( unsigned char* ) malloc( n*sizeof( unsigned char ) ) ) == NULL )
		{
			perror ( "Erreur de Mémoire (Malloc)" ) ;
			exit ( EXIT_FAILURE );
		}
	}	
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








