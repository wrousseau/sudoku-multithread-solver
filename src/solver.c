#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "structures.h"
#include "memory_handler.h"
#include "solver.h"
#include "inputoutput_handler.h"

extern Sudoku* sudoku;

void *threadStart(void* arg)
{
	int iDebug = 7;
	parThread* tab = arg;
	int n = tab->n, i = tab->i;
	unsigned char* result;
	//printf("%d\n", (int) tab[1]);
	initSubGrid( tab->subGrid , i, n );

	int widthSubSquare;
	widthSubSquare = (int) sqrt( n );

	while( sudoku -> emptyBlocks != 0 && i == iDebug)
	{//printf("grid :%d   global:%d\n",tab->subGrid->emptyBlocks, sudoku->emptyBlocks);
		
		
		result = searchChoices(tab -> subGrid, n, widthSubSquare, false);
		tab->subGrid->numberLaunch++;
		
		fillGrid(result, tab);//remplit la grid si besoin est
		usleep(1000000);
		/******* DEBUG ne pas supprimer*****/
		if(i == iDebug) 
		{ 
			for(int p = 0; p < widthSubSquare; p++)
			{
				for(int m = 0 ; m<widthSubSquare ; m++)
				{
					for(int q = 0 ; q < n; q++)
					{
						printf("%d ", (int) tab->subGrid->s[p][m].choices[q]);
					}
					printf("   >%d\n", (int) tab->subGrid->s[p][m].N_sol);
				}
			}printGrid();
			printf("grid :%d   global:%d\n",tab->subGrid->emptyBlocks, sudoku->emptyBlocks);
			//exit(0);
		}
		/********* ---- **********/

		tab->subGrid->emptyBlocks = sudoku->emptyBlocks;
		free(result);
		while(tab->subGrid->emptyBlocks == sudoku->emptyBlocks)//si il n'y a pas eu de modif, on attend
		{//printf("grid :%d   global:%d\n",tab->subGrid->emptyBlocks, sudoku->emptyBlocks);
			usleep(1000);
		}
		
		//writeGrid("sudoku_out.txt", sudoku->grid, 9);
	}

	return NULL;
}



unsigned char* searchChoices(subGrid* par, int n, int widthSubSquare, bool init)
{
	unsigned char* result;
	int resultPointer = 0;
	unsigned char tmp;
	if( (result = malloc(256*3*sizeof(unsigned char)) ) == NULL)
	{
		perror("Erreur de malloc ");
		exit ( EXIT_FAILURE );
	}
	for( int m = 0 ; m < widthSubSquare ; m++)
	{
		for( int p = 0 ; p < widthSubSquare ; p++)
		{	//printf("pute  m:%d  p:%d\n",m,p);
			//On analyse chaque case du sous carré
			tmp = checkBlock( &( par->s[m][p] ), par, par->y * widthSubSquare + m, par->x * widthSubSquare + p, init);

			if(tmp != 0) //si on trouve quelque chose on le rajoute dans result
			{
				result[3*resultPointer] = tmp; // la valeur
				result[3*resultPointer + 1] = par->y * widthSubSquare + m; // i
				result[3*resultPointer + 2] = par->x * widthSubSquare + p; // j
			}
		}
	}
	return result;
}


void fillGrid(unsigned char* result, parThread* tab)
{
	if(result[0] != 0)
	{
		int k=0;
		tab->subGrid->successLaunch++;
		while(sudoku->locked == true) // on attend que la grille soit unlocked
		{
			usleep(100);
		}
		sudoku->locked = true;
		while(result[3*k] != 0) //on écrit dans la grille tout ce qu'on a trouvé
		{
			printf("%d\n", (int) result[3*k] );
			sudoku->grid[ result[3*k + 1] ][ result[3*k + 2] ] = result[3*k];
			sudoku->emptyBlocks--;
			k++;
		}
		tab->subGrid->emptyBlocks = sudoku->emptyBlocks; // On met à jour notre copie de Nv
		sudoku->locked = false;
	}
	else
	{
		tab->subGrid->failLaunch++;
	}
	return;
}


unsigned char checkBlock(Solution *s, subGrid* thread, unsigned char i, unsigned char j, bool init)
{
	unsigned char n = sudoku->n;

	if(init)
	{
		unsigned char block;
		if((block = sudoku->grid[i][j]) != 0) //si on connait déjà la case
		{
			s->N_sol = 1;
			for( int k = 0 ; k < n ; k++)
			{
				s->choices[k] = 0;
			}
			s->choices[block - 1] = 1;//exemple : si il y a un 2 dans la case, la 2° case de choices sera à 1, les autres à 0
		}
		else
		{
			s->N_sol = n ;
			for( int k = 0 ; k < n ; k++)
			{
				s->choices[k] = 1;
			}
		}
		return 0;
	}

	else
	{
		unsigned char result;
		if( s->N_sol != 1)
		{
			result = getNaiveChoices(s, thread, i, j);

			if(result == 0)
			{
				//result = getSingletonChoices(s, thread, i, j);
			}
			return result;
		}
	}
	return 0;
}


unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j)
{
	unsigned char n = sudoku->n, result = 0;
	int widthSubSquare = (int) sqrt( n );

	for( int k = 0 ; k < n ; k++)//on procède aux tests ...
	{
		unsigned char tmp;
		tmp = sudoku -> grid[i][k];
		if( tmp != 0 && s->choices[tmp-1] != 0)//... Horizontal ...
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
			result = tmp;
		}

		tmp = sudoku -> grid[k][j];
		if( tmp != 0 && s->choices[tmp-1] != 0)//... Vertical ...
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
			result = tmp;
		}

		tmp = sudoku -> grid[thread->y * widthSubSquare + k/widthSubSquare][thread->x * widthSubSquare + k%widthSubSquare];
		if(tmp != 0 && s->choices[tmp-1] != 0)//... Dans le sous carré
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
			result = tmp;
		}printf("%d ",(int) result );
	}printf("     %d&%d\n", (int) result, (int) s->N_sol);
	if(s->N_sol == 1)
	{
		return result;
	}
	else
	{
		return 0;
	}
}


unsigned char getSingletonChoices(Solution* s, subGrid* thread, unsigned char i, unsigned char j)
{
	unsigned char n = sudoku->n;
	//int ik = -1, jk = -1, k=1;
	int widthSubSquare = (int) sqrt( n );

	/*while( ik == -1 || jk == -1) //on repère le point en haut à gauche du sous carré qui nous interesse
	{
		unsigned char tmp = widthSubSquare * k;
		if(i < tmp)
		{
			ik = tmp - widthSubSquare;
		}
		if(j < tmp)
		{
			jk = tmp - widthSubSquare;
		}
		k++;
	}*/

	char tmp[255] = {0};
	//unsigned char result;
	for( int k = 0; k < widthSubSquare ; k++) //on repère si un nombre n'a qu'une seule case possible dans le carré
	{
		for( int l = 0 ; l < widthSubSquare ; l++)
		{
			if(thread->s[k][l].N_sol != 1)
			{
				for( int m = 0 ; m < n ; m++)
				{printf("%d ", tmp[m]);
					tmp[m] = tmp[m] + thread->s[k][l].choices[m];
				}printf("\n");
			}				
		}
	}
	for( int k = 0 ; k < n ; k++) // on vérifie s'il y a un singleton dans le sous carré
	{printf("%d ", (int) tmp[k]);}printf("     :%d%d   :%d\n\n", thread->x, thread->y, s->N_sol);

	for( int k = 0 ; k < n ; k++) // on vérifie s'il y a un singleton dans le sous carré
	{
		if(tmp[k] == 1 && s->choices[k] == 1) 
		//si on en trouve un qui correspond au block analysé, on envoie la valeur (+1 car le tableau commence à 0)
		{
			s->N_sol = 1;
			for( int l = 0 ; l < n ; l++)
			{
				s->choices[l] = 0;
			}
			s->choices[k] = 1;

			return k+1; 
		}
	}

	return 0;
}

