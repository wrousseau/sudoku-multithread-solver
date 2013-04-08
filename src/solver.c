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
	threadParameters* tab = arg;

	int n = tab->numberOfBlocks, i = tab->threadNumber;
	unsigned char* result;
	initSubGrid( tab->subGrid , i, n );

	int widthSubSquare;
	widthSubSquare = (int) sqrt( n );

	while( sudoku -> emptyBlocks != 0 && i == iDebug)
	{
		tab->subGrid->emptyBlocks = sudoku->emptyBlocks; // On met à jour notre copie de Nv

		//searchChoices cherche toutes les nouvelles solutions à mettre dans grid, et stocke le(s) résultat(s) dans result
		//result : tableau regroupé en bloc de 3 unsigned char : value, i et j pour compléter la grille. Si value = 0 <=> pas de nouvelle valeur
		result = searchChoices(tab -> subGrid, n, widthSubSquare, false);
		
		fillGrid(result, tab);//remplit la grid si besoin est grâce aux résultats stockés dans result
		usleep(1000000);
		/******* DEBUG ne pas supprimer*****/
		if(i == iDebug) 
		{ 
			for(int p = 0 ; p < n ; p++)
			{
				printf("%d %d %d\n", result[3*p], result[3*p+1], result[3*p+2]);
			}
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
	if( (result = malloc((n+1)*3*sizeof(unsigned char)) ) == NULL) // on alloue l'espace maximum nécessaire à result (3 variables pour un résultat)
	{
		perror ( "Erreur de malloc" );
		exit ( EXIT_FAILURE );
	}
	for( int m = 0 ; m < widthSubSquare ; m++) // Pour chaque case, on cherche
	{
		for( int p = 0 ; p < widthSubSquare ; p++)
		{	
			//On analyse chaque case du sous carré, retourne la valeur de la case si on checkBlock l'a trouvé
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


void fillGrid(unsigned char* result, threadParameters* tab)
{
	tab->subGrid->numberLaunch++;

	if(result[0] != 0) // si le premier "value" est nul, c'est que l'on a pas de nouveau résultat
	{
		int k=0;
		tab->subGrid->successLaunch++;
		while(sudoku->locked == true) // on attend que la grille soit unlocked
		{
			usleep(100);
		}
		sudoku->locked = true; //dès que l'on a accès, on lock
		while(result[3*k] != 0) //on écrit dans la grille tout ce qu'on a trouvé (Tant que les "values" sont non nulles)
		{
			printf("%d\n", (int) result[3*k] ); //DEBUG
			sudoku->grid[ result[3*k + 1] ][ result[3*k + 2] ] = result[3*k]; // grid[i][j] = value
			sudoku->emptyBlocks--; // on décremente la variable globale emptyBlocks
			k++;
		}
		
		sudoku->locked = false;// on Unlock la grille globale
	}
	else
	{
		tab->subGrid->failLaunch++;
	}
	return;
}


unsigned char checkBlock(Solution *s, subGrid* thread, unsigned char i, unsigned char j, bool init)
{
	unsigned char n = sudoku->blocksPerSquare;

	/******** INITIALISATION *********/
	if(init) // si on est en initialisation (juste après la création du thread)
	{
		unsigned char block; // valeur d'une case
		if((block = sudoku->grid[i][j]) != 0) //si on connait déjà la case, on définit choices pour cette valeur et N_sol = 1
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
			s->N_sol = n ;
			for( int k = 0 ; k < n ; k++)
			{
				s->choices[k] = 1;
			}
		}
		return 0;
	}
	/************ CALCULS ************/
	else
	{
		unsigned char result;
		if( s->N_sol != 1) // Si N_sol = 1, on connait déjà la solution, on retourne alors 0 pour dire que l'on a rien trouvé de nouveau
		{
			// renvoie la valeur résultat si la case concerné n'a qu'un choix de nombre disponible, 0 sinon
			result = getNaiveChoices(s, thread, i, j); 

			if(result == 0)
			{
				// renvoie la valeur résultat si un nombre n'apparait qu'une fois dans un sous carré, 0 sinon
				//result = getSingletonChoices(s, thread, i, j);
			}
			return result;
		}
	}
	return 0;
}


unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j)
{
	unsigned char n = sudoku->blocksPerSquare, result = 0;
	int widthSubSquare = (int) sqrt( n );

	for( int k = 0 ; k < n ; k++)//on procède aux tests ...
	{
		unsigned char tmp;

		//... Horizontal ...
		tmp = sudoku -> grid[i][k];
		if( tmp != 0 && s->choices[tmp-1] != 0) // si on regarde une case connue dans le grille principale, et qu'on ne le savait pas ...
		{
			s->choices[tmp-1] = 0; // ... On élimine ce choix
			s->N_sol--; // on décrémente N_sol
		}

		//... Vertical ...
		tmp = sudoku -> grid[k][j];
		if( tmp != 0 && s->choices[tmp-1] != 0)
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
		}

		//... Dans le sous carré
		tmp = sudoku -> grid[thread->y * widthSubSquare + k/widthSubSquare][thread->x * widthSubSquare + k%widthSubSquare];
		if(tmp != 0 && s->choices[tmp-1] != 0)
		{
			s->choices[tmp-1] = 0;
			s->N_sol--;
		}printf("%d ",(int) result ); // debug
	}printf("     %d&%d\n", (int) result, (int) s->N_sol); //debug

	if(s->N_sol == 1)
	{
		// si on a plus qu'un choix, on trouvé la valeur. On recherche le seul choix disponible et on le renvoie 
		// (Attention, choices[0] correspond au nombre 1)
		for(int k = 0 ; k < n ; k++)
		{
			if(s->choices[k] != 0)
			{
				result = k+1;
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


unsigned char getSingletonChoices(Solution* s, subGrid* thread, unsigned char i, unsigned char j)
{
	unsigned char n = sudoku->blocksPerSquare;
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

	char tmp[255] = {0}; // dans tmp on met la somme des tableaux "choices" du sous carré où N_sol > 1
	//unsigned char result;

	//on repère si un nombre n'a qu'une seule case possible dans le carré. C'est le cas si une case de tmp est égale à 1 après toutes les sommes
	for( int k = 0; k < widthSubSquare ; k++) 
	{
		for( int l = 0 ; l < widthSubSquare ; l++)
		{
			if(thread->s[k][l].N_sol != 1)
			{
				for( int m = 0 ; m < n ; m++)
				{printf("%d ", tmp[m]);//debug
					tmp[m] = tmp[m] + thread->s[k][l].choices[m];
				}printf("\n");//debug
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

