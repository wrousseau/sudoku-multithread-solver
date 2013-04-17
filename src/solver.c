#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#include "structures.h"
#include "memory_handler.h"
#include "solver.h"
#include "inputoutput_handler.h"


extern Sudoku* sudoku;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *threadStart(void* arg)
{
	threadParameters* tab = (threadParameters*) arg;

	int counter = 0, wait = 0;
	unsigned char* result;
	initSubGrid( tab->subGrid , tab->threadNumber );
	initResult(&result );
	unsigned int test;
	usleep(test);
	while( sudoku -> emptyBlocks != 0 )
	{
		tab->subGrid->emptyBlocks = sudoku->emptyBlocks; // On met à jour notre copie de Nv

		// searchChoices cherche toutes les nouvelles solutions à mettre dans grid, et stocke le(s) résultat(s) dans result
		// result : tableau regroupant en bloc 3 unsigned char : value, l'abcisse et l'ordonnée de la case pour compléter la grille.
		// Si value = 0, pas de nouvelle valeur
		searchChoices(&result, tab -> subGrid);

		// Remplissage la grid si besoin est grâce aux résultats stockés dans result
		fillGrid(result, tab);

 		bool timeOut = false;
 		while(!timeOut)
 		{
			pthread_mutex_lock(&mut);
			if ( pthread_cond_timedwait(&cond, &mut, &(tab->timedwaitExpiration) ) == ETIMEDOUT )
			{
				timeOut = true;
			}
			pthread_mutex_unlock(&mut);
		}

	}
	free(result);

	return NULL;
}

void searchChoices(unsigned char **result , subGrid* currentSubGrid)
{
	int resultPointer = 0;
	unsigned char tmp, numberOfBlocks, subSquareWidth;
	numberOfBlocks = sudoku->blocksPerSquare;
	subSquareWidth = (unsigned char) sqrt(numberOfBlocks);
	
	for( int i = 0 ; i < subSquareWidth ; i++) // Pour chaque case, on cherche
	{
		for( int j = 0 ; j < subSquareWidth ; j++)
		{	
			// On analyse chaque case du sous carré, retourne la valeur de la case si on checkBlock l'a trouvÃ©
			tmp = checkBlock( &( currentSubGrid->solution[i][j] ) , 
							  currentSubGrid , 
							  currentSubGrid->y * subSquareWidth + i , 
							  currentSubGrid->x * subSquareWidth + j );

			if(tmp != 0) // Si on trouve une solution, on la rajoute dans result
			{
				(*result)[3*resultPointer] = tmp; // la valeur
				(*result)[3*resultPointer + 1] = currentSubGrid->y * subSquareWidth + i; // On stocke "l'ordonnÃ©e" de la case
				(*result)[3*resultPointer + 2] = currentSubGrid->x * subSquareWidth + j; // On stocke "l'abcisse" de la case
				resultPointer++;
			}
		}
	}
	(*result)[3*resultPointer] = 0; // On marque la fin de result

	if(currentSubGrid->numberLaunch == 0) // On compte les solutions au démarrage après le premier calcul
	{
		countSolution(currentSubGrid);
	}
}

void initChoices( subGrid* currentSubGrid )
{
	int subSquareWidth = (int) sqrt(sudoku->blocksPerSquare);

	for( int i = 0 ; i < subSquareWidth ; i++) // Pour chaque case, on cherche
	{
		for( int j = 0 ; j < subSquareWidth ; j++)
		{	
			Solution *s = &( currentSubGrid->solution[i][j] );
			int y = currentSubGrid->y * subSquareWidth + i;
			int x = currentSubGrid->x * subSquareWidth + j;
			unsigned char n = sudoku->blocksPerSquare;

			unsigned char block; // valeur d'une case
			if ( ( block = sudoku->grid[y][x] ) != 0) //si on connait déjà la case, on définit choices pour cette valeur et N_sol = 1
			{
				s->N_sol = 1;
				for( int k = 0 ; k < n ; k++)
				{
					s->choices[k] = 0;
				}
				s->choices[block - 1] = 1;//exemple : si il y a un 2 dans la case, la 2° case de choices sera à 1, les autres à 0
			}
			else // Si on ne connait pas la valeur (0), on active tous les choix et on met N_sol à n
			{
				currentSubGrid->emptyAtBoot++; // Stats
				s->N_sol = n ;
				for( int k = 0 ; k < n ; k++)
				{
					s->choices[k] = 1;
				}
			}
		}
	}
}


void fillGrid(unsigned char* result, threadParameters* tab)
{
	tab->subGrid->numberLaunch++;

	if(result[0] != 0) // si le premier "value" est nul, c'est que l'on a pas de nouveau résultat
	{
		int k=0;
		tab->subGrid->successLaunch++;

		pthread_mutex_lock (&(sudoku->mutex)); /* on attent de pouvoir ouvrir le verrou , le thread d'écriture est lancé avant la lecture */
		while(result[3*k] != 0) //on écrit dans la grille tout ce qu'on a trouvé (Tant que les "values" sont non nulles)
		{
			sudoku->grid[ result[3*k + 1] ][ result[3*k + 2] ] = result[3*k]; // grid[i][j] = value
			sudoku->emptyBlocks--; // on décremente la variable globale emptyBlocks
			pthread_mutex_lock(&mut);
			pthread_cond_broadcast(&cond);
			pthread_mutex_unlock(&mut);
			k++;
		}

		pthread_mutex_unlock(&(sudoku->mutex)); /* on attent de pouvoir ouvrir le verrou , le thread d'écriture est lancé avant la lecture */
	}
	else
	{
		tab->subGrid->failLaunch++;
	}
	return;
}


unsigned char checkBlock(Solution *s, subGrid* subGrid, unsigned char yGlobal, unsigned char xGlobal)
{
	unsigned char result=0;
	int subSquareWidth = (int)sqrt(sudoku->blocksPerSquare);
	if( s->N_sol != 1) // Si N_sol = 1, on connait déjà la solution, on retourne alors 0 pour dire que l'on a rien trouvé de nouveau
	{
		// renvoie la valeur résultat si la case concerné n'a qu'un choix de nombre disponible, 0 sinon
		result = getNaiveChoices(s, subGrid, yGlobal, xGlobal);

		if(result != 0) // Alors on efface ce choix des possibilités ...
		{
			for(int p =0 ; p < subSquareWidth ; p++)
			{
				for(int q=0 ; q < subSquareWidth ; q++)
				{	
					// ... sauf pour la case concernée évidemment
					if( !( (yGlobal % subSquareWidth == p) && (xGlobal % subSquareWidth == q) ) )
					{
						subGrid->solution[p][q].choices[result - 1] = 0; // Donc ce choix n'est plus possible dans les autres cases
					}
				}
			}
		}
		else
		{
			// renvoie la valeur résultat si un nombre n'apparait qu'une fois dans un sous carré, 0 sinon
			result = getSingletonChoices(s, subGrid, yGlobal, xGlobal);
		}
		return result;
	}
	return 0;
}


unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char yGlobal, unsigned char xGlobal)
{
	unsigned char n = sudoku->blocksPerSquare, result = 0;
	int subSquareWidth = (int) sqrt( n );

	for( int i = 0 ; i < n ; i++)//on procède aux tests ...
	{
		unsigned char tmp;

		//... Horizontal ...
		tmp = sudoku -> grid[yGlobal][i];
		if( tmp != 0 && s->choices[tmp-1] != 0) // si on regarde une case connue dans le grille principale, et qu'on ne le savait pas ...
		{
			s->choices[tmp-1] = 0; // ... On élimine ce choix
			s->N_sol--; // on décrémente N_sol
		}

		//... Vertical ...
		tmp = sudoku -> grid[i][xGlobal];
		if( tmp != 0 && s->choices[tmp-1] != 0)
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
		}

		//... Dans le sous carré
		tmp = sudoku -> grid[thread->y * subSquareWidth + i/subSquareWidth][thread->x * subSquareWidth + i%subSquareWidth];
		if(tmp != 0 && s->choices[tmp-1] != 0)
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
		}
	}

	if(s->N_sol == 1)
	{
		// si on a plus qu'un choix, on trouvé la valeur. On recherche le seul choix disponible et on le renvoie 
		// (Attention, choices[0] correspond au nombre 1)
		for(int i = 0 ; i < n ; i++)
		{
			if(s->choices[i] != 0)
			{
				result = i+1;
				break;
			}
		}
		return result; 
	}
	else // Sinon on a pas de certitude, on renvoie 0
	{
		return 0;
	}
}


unsigned char getSingletonChoices(Solution* s, subGrid* thread, unsigned char yGlobal, unsigned char xGlobal)
{
	unsigned char n = sudoku->blocksPerSquare;
	int subSquareWidth = (int) sqrt( sudoku->blocksPerSquare );

	char tmp[255] = {0}; // dans tmp on met la somme des tableaux "choices" du sous carré où N_sol > 1

	//on repère si un nombre n'a qu'une seule case possible dans le carré. C'est le cas si une case de tmp est égale à 1 après toutes les sommes
	for( int i = 0; i < subSquareWidth ; i++) 
	{
		for( int j = 0 ; j < subSquareWidth ; j++)
		{
			if(thread->solution[i][j].N_sol != 1) // On ne prend pas en compte les cases qu'on connait déjà, car ce sont des singletons peu interessants
			{
				for( int k = 0 ; k < n ; k++) // On fait la somme sur tout le tableau
				{
					tmp[k] = tmp[k] + thread->solution[i][j].choices[k];
				}
			}				
		}
	}
	
	for( int i = 0 ; i < n ; i++) // On vérifie s'il y a un singleton dans le sous carré
	{
		if(tmp[i] == 1 && s->choices[i] == 1) 
		// Si on en trouve un qui correspond au block analysé, on envoie la valeur (+1 car le tableau commence à 0)  
		{
			// On met tout comme il faut dans choices et N_sol
			s->N_sol = 1;
			for( int j = 0 ; j < n ; j++)
			{
				s->choices[j] = 0;
			}
			s->choices[i] = 1;

			for(int p = 0 ; p < n ; p++) // on regarde si une case avec cette valeur n'existe pas déjà dans le sous carré
			{
				if(sudoku -> grid[yGlobal][xGlobal] == i+1)
				{
					return 0; // dans ce cas, notre résultat est faux : on renvoie 0
				}
			}

			return i+1; // On retourne la valeur trouvée de la case
		}
	}

	return 0;
}


void countSolution(subGrid* currentSubGrid)	
{
	int subSquareWidth = sqrt(sudoku->blocksPerSquare);
	currentSubGrid->solAtBoot = 0;

	for(int i = 0 ; i < subSquareWidth ; i++)
	{
		for(int j = 0 ; j < subSquareWidth ; j++)
		{
			if(currentSubGrid->solution[i][j].N_sol != 1) // si on connait la case au début, ça ne compte pas
			{
				for(int k = 0 ; k < sudoku->blocksPerSquare ; k++) //on regarde le nombre de solution par case
				{
					if(currentSubGrid->solution[i][j].choices[k] == 1)
					{
						currentSubGrid->solAtBoot++;
					}
				}
			}
		}
	}
}
