#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include "structures.h"
#include "solver.h"

extern Sudoku* sudoku;

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
		if( pthread_create( &( (*threadsAdress)[i].thread ) , NULL, threadStart , &(arg[i]) ) != 0 )
        {
                perror( "Erreur dans pthread_create" );
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
	if( (par -> s = (Solution**) malloc(widthSubSquare * sizeof (Solution*)) ) == NULL) // création de la matrice Solution en local
	{
		perror("Problème de Malloc ");
		exit ( EXIT_FAILURE );
	} 
	for (int k = 0 ; k < widthSubSquare ; k++)
	{
		if( (par -> s[k] = (Solution*) malloc(widthSubSquare * sizeof (Solution)) ) == NULL)
		{
			perror("Problème de Malloc ");
			exit ( EXIT_FAILURE );
		} 
	}

	par -> emptyBlocks = sudoku -> emptyBlocks;
	//printf("i : %d  ||  n : %d\n", i, n);
	//printf("x : %d  ||  y : %d\n", par->x, par->y);

	for( int m = 0 ; m < widthSubSquare ; m++)
	{
		for( int p = 0 ; p < widthSubSquare ; p++)
		{	
			unsigned char block = sudoku -> grid[par->x * widthSubSquare + m][par->y * widthSubSquare + p];

			if(block != 0) // si la case en question est définie, le seul choix possible s'impose
			{
				par -> s[m][p].N_sol = 1;
				for( int k = 0 ; k < n ; k++)
				{
					par -> s[m][p].choices[k] = 0;
				}
				par -> s[m][p].choices[block - 1] = 1;//exemple : si il y a un 2 dans la case, la 2° case de choices sera à 1, les autres à 0
			}
			else // sinon on cherche les possibilités
			{
				for( int k = 0 ; k < n ; k++)
				{
					par -> s[m][p].choices[k] = 1; //pour le moment tous les choix sont possibles jusqu'à n
				}
				par -> s[m][p].N_sol = n;

				for( int k = 0 ; k < n ; k++)//on procède au tests ...
				{
					unsigned char tmp;
					tmp = sudoku -> grid[par->x * widthSubSquare + m][k];
					if( tmp !=0 && par -> s[m][p].choices[tmp-1] != 0)//... Horizontal ...
					{
						par -> s[m][p].choices[tmp-1] = 0;
						par -> s[m][p].N_sol--;
					}
					tmp = sudoku -> grid[k][par->y * widthSubSquare + p];
					if( tmp !=0 && par -> s[m][p].choices[tmp-1] != 0)//... Vertical ...
					{
						par -> s[m][p].choices[tmp-1] = 0;
						par -> s[m][p].N_sol--;
					}
					tmp = sudoku -> grid[par->x * widthSubSquare + k/widthSubSquare][par->y * widthSubSquare + k%widthSubSquare];
					if(tmp !=0 && par -> s[m][p].choices[tmp-1] != 0)//... Dans le sous carré
					{
						par -> s[m][p].choices[tmp-1] = 0;
						par -> s[m][p].N_sol--;
					}

				}
			}
		}
	}

	return;
}


