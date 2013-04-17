#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "structures.h"
#include "solver.h"
#include "inputoutput_handler.h"
#include "memory_handler.h"

extern Sudoku* sudoku;

void createGrid ( unsigned char blocksPerSquare , unsigned char*** gridAdress )
{
	if ( ( *gridAdress = ( unsigned char** ) malloc( blocksPerSquare*sizeof( unsigned char* ) ) ) == NULL )
	{
		perror( "Malloc" );
		exit( EXIT_FAILURE );
	}
	
	for( int i = 0 ; i < blocksPerSquare ; i++)
	{
		if ( ( (*gridAdress)[i] = ( unsigned char* ) malloc( blocksPerSquare*sizeof( unsigned char ) ) ) == NULL )
		{
			perror ( "Malloc" ) ;
			exit ( EXIT_FAILURE );
		}
	}	
}

void deleteSudoku()
{
	for( int i = 0; i < sudoku->blocksPerSquare ; i++)
	{
		free(sudoku->grid[i]);
	}
	free(sudoku->grid);
	free(sudoku);
}


void initSudoku( unsigned char** grid, unsigned char blocksPerSquare)
{
	
	sudoku = malloc (sizeof( Sudoku ));
	if(sudoku == NULL)
	{
		perror( "Erreur de Mémoire (Malloc)" );
		exit( EXIT_FAILURE );
	}	
	//Calcul de emptyBlocks
	int i, j;
	sudoku->emptyBlocks = 0;
	sudoku->blocksPerSquare = blocksPerSquare;
	sudoku->grid = grid;
	sudoku->locked = false;
	
	for(i = 0 ; i < sudoku->blocksPerSquare ; i++)
	{
		for(j = 0 ; j < sudoku->blocksPerSquare ; j++)
		{
			if(grid[i][j] == 0)
			{
				sudoku->emptyBlocks++;
			}
		}
	}
	return;
}


void launchThreads( subGrid** subGridsAdresses )
{
	int blocksPerSquare = sudoku->blocksPerSquare;
	threadParameters *arg;
	arg = malloc ( blocksPerSquare*sizeof(threadParameters) );
	if ( arg == NULL )
	{
		perror( "Problème de Mémoire (Malloc)" );
		exit ( EXIT_FAILURE );
	}

	*subGridsAdresses = malloc( blocksPerSquare * sizeof( subGrid ) );
	if( *subGridsAdresses == NULL )
	{
		perror( "Erreur de Mémoire (Malloc)" );
		exit( EXIT_FAILURE );
	}

	printf ( "Lancement de %d threads\n" , blocksPerSquare );
	for( int i = 0 ; i < blocksPerSquare ; i++ )
	{
		arg[i].threadNumber = i;
		arg[i].numberOfBlocks = blocksPerSquare;
		arg[i].subGrid = &( ( *subGridsAdresses )[i] ); //Adresse de la subGrid concernée
		if ( pthread_create( &( (*subGridsAdresses)[i].thread ) , NULL, threadStart , &(arg[i]) ) != 0 )
        {
                perror( "Erreur dans pthread_create" );
                exit( EXIT_FAILURE );
        }

	}
	for( int i = 0 ; i < blocksPerSquare ; i++ )
	{
		if ( pthread_join ( (*subGridsAdresses)[i].thread , NULL ) != 0 )
        {
                perror ( "Erreur dans pthread_join" );
                exit ( EXIT_FAILURE );
        }
        printStatsThread( &(arg[i]), 0);
        cleanSubGrids( &(arg[i]));
	}
	free(arg);
    return;
}


void initSubGrid( subGrid* subGrid , int threadNumber )
{
	int widthSubSquare, numberOfBlocks = sudoku->blocksPerSquare;
	widthSubSquare = (int) sqrt( numberOfBlocks );
	subGrid -> x = threadNumber % widthSubSquare;
	subGrid -> y = threadNumber / widthSubSquare;
	subGrid -> successLaunch = 0;
	subGrid -> failLaunch = 0;
	subGrid -> numberLaunch = 0;
	subGrid -> solAtBoot = 0;
	subGrid -> emptyAtBoot = 0;
	if( ( subGrid -> solution = (Solution**) malloc(widthSubSquare * sizeof (Solution*)) ) == NULL) // création de la matrice Solution en local
	{
		perror("Problème de Malloc ");
		exit ( EXIT_FAILURE );
	} 
	for (int i = 0 ; i < widthSubSquare ; i++)
	{
		if( ( subGrid -> solution[i] = (Solution*) malloc(widthSubSquare * sizeof (Solution)) ) == NULL)
		{
			perror("Problème de Malloc ");
			exit ( EXIT_FAILURE );
		} 
	}

	initChoices( subGrid ); // solver.c
}

void initResult(unsigned char **result)
{
	// On alloue l'espace maximum nécessaire à result (3 variables pour un résultat)
	if( (*result = malloc((sudoku->blocksPerSquare + 1)*3*sizeof(unsigned char)) ) == NULL)
	{
		perror ( "Malloc : " );
		exit ( EXIT_FAILURE );
	}
}


void cleanSubGrids( threadParameters* parameters)
{
	int widthSubSquare = sqrt(sudoku->blocksPerSquare);

	for(int i = 0 ; i < widthSubSquare ; i++)
	{
		free(parameters->subGrid->solution[i]);
	}
	free(parameters->subGrid->solution);
	//free(parameters->subGrid);
	//free(parameters);
	
	return;
}




