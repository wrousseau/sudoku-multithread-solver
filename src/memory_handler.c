#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "structures.h"
#include "solver.h"
#include "inputoutput_handler.h"

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

void deleteSudoku(Sudoku* sudoku)
{
	for( int i = 0; i < sudoku->blocksPerSquare ; i++)
	{
		free(sudoku->grid[i]);
	}
	free(sudoku->grid);
	free(sudoku);
}


void initSudoku( Sudoku** sudokuAdress, unsigned char** grid, unsigned char blocksPerSquare)
{
	
	*sudokuAdress = malloc (sizeof( Sudoku ));
	if(sudoku == NULL)
	{
		perror( "Erreur de Mémoire (Malloc)" );
		exit( EXIT_FAILURE );
	}	
	//Calcul de emptyBlocks
	int i, j;
	(*sudokuAdress)->emptyBlocks = 0;
	(*sudokuAdress)->blocksPerSquare = blocksPerSquare;
	(*sudokuAdress)->grid = grid;
	(*sudokuAdress)->locked = false;
	
	for(i = 0 ; i < (*sudokuAdress)->blocksPerSquare ; i++)
	{
		for(j = 0 ; j < (*sudokuAdress)->blocksPerSquare ; j++)
		{
			if(grid[i][j] == 0)
			{
				(*sudokuAdress)->emptyBlocks++;
			}
		}
	}
	return;
}


void launchThreads( subGrid** threadsAdresses, int blocksPerSquare)
{
	threadParameters *arg;
	arg = malloc ( blocksPerSquare*sizeof(threadParameters) );
	if ( arg == NULL )
	{
		perror( "Problème de Mémoire (Malloc)" );
		exit ( EXIT_FAILURE );
	}

	*threadsAdresses = malloc( blocksPerSquare * sizeof( subGrid ) );
	if( *threadsAdresses == NULL )
	{
		perror( "Erreur de Mémoire (Malloc)" );
		exit( EXIT_FAILURE );
	}

	printf ( "Lancement de %d threads\n" , blocksPerSquare );
	for( int i = 0 ; i < blocksPerSquare ; i++ )
	{
		arg[i].threadNumber = i;
		arg[i].numberOfBlocks = blocksPerSquare;
		arg[i].subGrid = &( ( *threadsAdresses )[i] ); //Adresse de la subGrid concernée
		if ( pthread_create( &( (*threadsAdresses)[i].thread ) , NULL, threadStart , &(arg[i]) ) != 0 )
        {
                perror( "Erreur dans pthread_create" );
                exit( EXIT_FAILURE );
        }

	}
	for( int i = 0 ; i < blocksPerSquare ; i++ )
	{
		if ( pthread_join ( (*threadsAdresses)[i].thread , NULL ) != 0 )
        {
                perror ( "Erreur dans pthread_join" );
                exit ( EXIT_FAILURE );
        }
        printStatsThread( &(arg[i]), 0);
	}
    return;
}


void initSubGrid( subGrid* subGrid , int threadNumber, int numberOfBlocks )
{
	int widthSubSquare;
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

	initChoices( subGrid, widthSubSquare ); // solver.c
}

void initResult(unsigned char **result, int numberOfBlocks)
{
	// On alloue l'espace maximum nécessaire à result (3 variables pour un résultat)
	if( (*result = malloc((numberOfBlocks+1)*3*sizeof(unsigned char)) ) == NULL)
	{
		perror ( "Malloc : " );
		exit ( EXIT_FAILURE );
	}
}





