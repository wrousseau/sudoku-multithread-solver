/**
 * \file main.c
 * \brief Programme Multi-Thread de Résolution de Grilles de Sudoku
 * \author Simon Manchel & Woody Rousseau
 * \version 0.1
 * \date 13 avril 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include "structures.h"
#include "memory_handler.h"
#include "inputoutput_handler.h"
#include "solver.h"

// Le pointeur vers le Sudoku est une variable globale
Sudoku* sudoku;

/**
 * \fn void displayUsage()
 * \brief Affiche comment doit être utilisée le programme (Usage)
 * \return EXIT_SUCCESS - Arrêt normal du programme.
 */
void displayUsage();

/**
 * \fn int main (int argc, char **argv)
 * \brief Entrée du programme
 * \param argc Nombre d'arguments passés au programme (en comptant son nom)
 * \param argv Tableau des chaînes de caractères passées en argument au programme
 * \return EXIT_SUCCESS - Arrêt normal du programme.
 */
int main (int argc, char **argv)
{
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
	
	if ( ( argc >= 2) && (strcmp ( argv[1] , "help\n" ) == 0) ) 
	{
		displayUsage();
		exit ( EXIT_SUCCESS );
	}

	struct timeval timeStart, timeEnd;
	double time_spent;
	gettimeofday( &timeStart, NULL);

	unsigned char** grid; // Déclaration du double pointeur de la matrice représentant la grille de jeu
	unsigned char blocksPerSquare;
	subGrid* threads = NULL;

	char *filePath = "sudoku.in.txt", *resultPath = "sudoku.out.txt";
	switch (argc)
	{
		case 1:
			break;
		case 2:
			filePath = argv[1];
			break;
		case 3:
			filePath = argv[1];
			resultPath = argv[2];
			break;
		default:
			displayUsage();
	}

	blocksPerSquare = readDimensions(filePath);
	createGrid(blocksPerSquare, &grid);
	readGrid(filePath, grid, blocksPerSquare);
	initSudoku( grid, blocksPerSquare);
	launchThreads( &threads, blocksPerSquare);
	
	writeGrid(resultPath, grid, blocksPerSquare);
	deleteSudoku(sudoku);

	gettimeofday( &timeEnd, NULL);
	time_spent = ((double) (timeEnd.tv_usec - timeStart.tv_usec)) / 1000000;
	printStatsThread(NULL, time_spent);
	printf("Temps d'Exécution : %f secondes\n" ,time_spent);
	exit( EXIT_SUCCESS );
}

void displayUsage()
{
	fprintf(stdout , "Usage :\n");
	fprintf(stdout, "\"./exe/sudoku\" / \"make run\" : Default avec \"sudoku.in.txt\" en entrée.\n");
	fprintf(stdout, "\"./exe/sudoku [INPUT]\" : Grille d\'entrée spécifiée.\n");
	fprintf(stdout, "\"./exe/sudoku [INPUT] [OUTPUT]\" : Grilles d\'entrée et de sortie spécifiées.\n");
	exit ( EXIT_SUCCESS );
}
