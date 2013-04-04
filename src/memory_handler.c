#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "structures.h"
#include "solver.h"

extern Sudoku* sudoku;

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
	sudoku->locked = false;
	
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


void launchThreads( subGrid** threadsAdress, int n)
{
	parThread *arg;
	arg = malloc(n * sizeof(parThread));
	if(arg == NULL)
	{
		perror("Problème de Malloc ");
		exit ( EXIT_FAILURE );
	}

	(*threadsAdress) = malloc( n * sizeof( subGrid ) );
	if(*threadsAdress == NULL)
	{
		perror("Erreur de Malloc ");
		exit( EXIT_FAILURE );
	}

	printf("Lancement de %d threads\n", n);
	for( int i = 0 ; i < n ; i++ )
	{
		arg[i].i = i;
		arg[i].n = n;
		arg[i].subGrid = &((*threadsAdress)[i]); //Adresse de la subGrid concernée
		//printf("Plouf : %d\n", tab[2]);
		//initSubGrid( &((*threadsAdress)[i]), i, n );
		if(pthread_create( &((*threadsAdress)[i].thread) , NULL, threadStart , &(arg[i])) != 0)
        {
                perror("Erreur dans pthread_create");
                exit( EXIT_FAILURE );
        }

	}
	for( int i = 0 ; i < n ; i++ )
	{
		if(pthread_join( (*threadsAdress)[i].thread, NULL ) != 0)
        {
                perror("Erreur dans pthread_join");
                exit( EXIT_FAILURE );
        }
	}printf("test\n");
    return;
}


void initSubGrid( subGrid* par , int i, int n )
{
	int widthSubSquare;
	widthSubSquare = (int) sqrt( n );
	par -> x = i % widthSubSquare;
	par -> y = i / widthSubSquare;
	par -> successLaunch = 0;
	par -> failLaunch = 0;
	par -> numberLaunch = 0;

	par -> emptyBlocks = sudoku -> emptyBlocks;
	//printf("i : %d  ||  n : %d\n", i, n);
	printf("x : %d  ||  y : %d\n", par->x, par->y);



	return;
}


