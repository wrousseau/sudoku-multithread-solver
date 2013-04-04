#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "structures.h"
#include "memory_handler.h"
#include "solver.h"

extern Sudoku* sudoku;

void *threadStart(void* arg)
{
	parThread* tab = arg;
	int n = tab->n, i = tab->i;
	unsigned char* result;
	//printf("%d\n", (int) tab[1]);
	initSubGrid( tab->subGrid , i, n );

	int widthSubSquare;
	widthSubSquare = (int) sqrt( n );

	while( sudoku -> emptyBlocks != 0)
	{
		result = searchChoices(tab -> subGrid, n, widthSubSquare);
		tab->subGrid->numberLaunch++;
		if(result[0] != 0)
		{
			int k=0;
			tab->subGrid->successLaunch++;
		}
		else
		{
			tab->subGrid->failLaunch++;
		}
		
		free(result);
	}

	return NULL;
}



unsigned char* searchChoices(subGrid* par, int n, int widthSubSquare)
{
	unsigned char* result;
	int resultPointer = 0;
	if( (result = malloc(255*3*sizeof(unsigned char)) ) == NULL)
	{
		perror("Erreur de malloc ");
		exit ( EXIT_FAILURE );
	}
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
						result[resultPointer*3] = tmp;
						result[resultPointer*3 + 1] = par->x * widthSubSquare + m;
						result[resultPointer*3 + 2] = par->y * widthSubSquare + p;
						resultPointer++;
					}

					tmp = sudoku -> grid[k][par->y * widthSubSquare + p];
					if( tmp !=0 && par -> s[m][p].choices[tmp-1] != 0)//... Vertical ...
					{
						par -> s[m][p].choices[tmp-1] = 0;
						par -> s[m][p].N_sol--;
						result[resultPointer*3] = tmp;
						result[resultPointer*3 + 1] = par->x * widthSubSquare + m;
						result[resultPointer*3 + 2] = par->y * widthSubSquare + p;
						resultPointer++;
					}

					tmp = sudoku -> grid[par->x * widthSubSquare + k/widthSubSquare][par->y * widthSubSquare + k%widthSubSquare];
					if(tmp !=0 && par -> s[m][p].choices[tmp-1] != 0)//... Dans le sous carré
					{
						par -> s[m][p].choices[tmp-1] = 0;
						par -> s[m][p].N_sol--;
						result[resultPointer*3] = tmp;
						result[resultPointer*3 + 1] = par->x * widthSubSquare + m;
						result[resultPointer*3 + 2] = par->y * widthSubSquare + p;
						resultPointer++;
					}
					result[resultPointer*3]=0;//on marque la fin de la liste

				}
			}
		}
	}
	return result;
}
