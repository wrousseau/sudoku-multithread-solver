#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#include "structures.h"
#include "memory_handler.h"
#include "solver.h"
#include "inputoutput_handler.h"


extern Sudoku* sudoku;
static pthread_mutex_t signalMutex; 


void *threadStart(void* arg)
{
	int iDebug = 4;
	threadParameters* tab = (threadParameters*) arg;

	int n = tab->numberOfBlocks, i = tab->threadNumber, counter = 0, wait = 0;
	unsigned char* result;
	initSubGrid( tab->subGrid , tab->threadNumber, tab->numberOfBlocks );

	int subSquareWidth;
	subSquareWidth = (int) sqrt( tab->numberOfBlocks );

	while( sudoku -> emptyBlocks != 0 )//&& i == iDebug)
	{
		tab->subGrid->emptyBlocks = sudoku->emptyBlocks; // On met à jour notre copie de Nv

		//searchChoices cherche toutes les nouvelles solutions à mettre dans grid, et stocke le(s) résultat(s) dans result
		//result : tableau regroupant en bloc 3 unsigned char : value, l'abcisse et l'ordonnée de la case pour compléter la grille. Si value = 0, pas de nouvelle valeur
		initResult(&result , tab -> numberOfBlocks);// ??????
		searchChoices(&result, tab -> subGrid, tab->numberOfBlocks, subSquareWidth);

		// Remplissage la grid si besoin est grâce aux résultats stockés dans result
		fillGrid(result, tab);
		/******* DEBUG ne pas supprimer*****/
		if ( 1)//i == iDebug ) 
		{ 
			/*for(int p = 0 ; p < n ; p++)
			{
				printf ( "%d %d %d\n" , result[3*p] , result[3*p+1] , result[3*p+2] );
			}
			for ( int p = 0; p < subSquareWidth; p++)
			{
				for ( int m = 0 ; m < subSquareWidth ; m++)
				{
					for ( int q = 0 ; q < n; q++)
					{
						printf("%d ", (int) tab->subGrid->solution[p][m].choices[q]);
					}
					printf("   >%d\n", (int) tab->subGrid->solution[p][m].N_sol);
				}
			}*/
			printGrid();
			printf("grid%d :%d   global:%d\n", i, tab->subGrid->emptyBlocks, sudoku->emptyBlocks);
		}
		/********* ---- **********/

		free(result);

		struct sigaction act;

		/* une affectation de pointeur de fonction */
		/* c’est equivalent d’ecrire act.sa_handler = &TraiteSignal */
		act.sa_handler = SIG_IGN;
		/* le masque (ensemble) des signaux non pris en compte est mis */
		/* a l’ensemble vide (aucun signal n’est ignore) */
		sigemptyset(&act.sa_mask);
		/* Les appels systemes interrompus par un signal */
		/* seront repris au retour du gestionnaire de signal */
		act.sa_flags = SA_RESTART;
		/* enregistrement de la reaction au SIGUSR1 */
		if ( sigaction(SIGUSR1,&act,NULL) == -1 ) 
		{
		/* perror permet d’afficher la chaine avec */
		/* le message d’erreur de la derniere commande */
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
		for(;;)
		{
		sigset_t mask;
		sigemptyset(&mask);
		sigaddset(&mask, SIGUSR1);
		sigsuspend(&mask);
		}

		/*while(tab->subGrid->emptyBlocks == sudoku->emptyBlocks && counter > 0 && wait <20) // Si il n'y a pas eu de modif, on attend
		{
			wait++;
			usleep(1000);
		}
		wait = 0;
		counter++; */
	}

	return NULL;
}

void searchChoices(unsigned char **result , subGrid* currentSubGrid, int numberOfBlocks, int subSquareWidth)
{
	int resultPointer = 0;
	unsigned char tmp;
	
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
}

void initChoices(subGrid* currentSubGrid, int subSquareWidth)
{
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
	currentSubGrid->solAtBoot = currentSubGrid->emptyAtBoot; // Stats
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
			pthread_mutex_lock (&(signalMutex)); /* on attent de pouvoir ouvrir le verrou , le thread d'écriture est lancé avant la lecture */
			if ( kill(getpid(),SIGUSR1) == -1 )
			{
				perror( "Envoi du signal");
				exit ( EXIT_FAILURE );
			}
			pthread_mutex_unlock(&(signalMutex)); /* on attent de pouvoir ouvrir le verrou , le thread d'écriture est lancé avant la lecture */

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


unsigned char checkBlock(Solution *s, subGrid* subGrid, unsigned char y, unsigned char x)
{
	unsigned char result;
	if( s->N_sol != 1) // Si N_sol = 1, on connait déjà la solution, on retourne alors 0 pour dire que l'on a rien trouvé de nouveau
	{
		// renvoie la valeur résultat si la case concerné n'a qu'un choix de nombre disponible, 0 sinon
		result = getNaiveChoices(s, subGrid, y, x);printf("NAIVE\n");
		if(result == 0)
		{
			// renvoie la valeur résultat si un nombre n'apparait qu'une fois dans un sous carré, 0 sinon
			//result = getSingletonChoices(s, subGrid, y, x);printf("SINGLETON\n");
		}
		return result;
	}
	return 0;
}


unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char y, unsigned char x)
{
	unsigned char n = sudoku->blocksPerSquare, result = 0;
	int subSquareWidth = (int) sqrt( n );

	for( int i = 0 ; i < n ; i++)//on procède aux tests ...
	{
		unsigned char tmp;

		//... Horizontal ...
		tmp = sudoku -> grid[y][i];
		if( tmp != 0 && s->choices[tmp-1] != 0) // si on regarde une case connue dans le grille principale, et qu'on ne le savait pas ...
		{
			s->choices[tmp-1] = 0; // ... On élimine ce choix
			s->N_sol--; // on décrémente N_sol
			thread->solAtBoot++; // Stats
		}

		//... Vertical ...
		tmp = sudoku -> grid[i][x];
		if( tmp != 0 && s->choices[tmp-1] != 0)
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
			thread->solAtBoot++; // Stats
		}

		//... Dans le sous carré
		tmp = sudoku -> grid[thread->y * subSquareWidth + i/subSquareWidth][thread->x * subSquareWidth + i%subSquareWidth];
		if(tmp != 0 && s->choices[tmp-1] != 0)
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
			thread->solAtBoot++; // Stats
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


unsigned char getSingletonChoices(Solution* s, subGrid* thread, unsigned char y, unsigned char x)
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

			for(int p = 0 ; p < subSquareWidth ; p++) // on regarde si une case avec cette valeur n'existe pas déjà dans le sous carré
			{
				if(sudoku -> grid[y * subSquareWidth + p/subSquareWidth][x * subSquareWidth + p%subSquareWidth] == i+1)
				{
					return 0; // dans ce cas, notre résultat est faux : on renvoie 0
				}
			}

			return i+1; // On retourne la valeur trouvée de la case
		}
	}

	return 0;
}

