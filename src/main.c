/**
 * \file main.c
 * \brief Programme Multi-Thread de Résolution de Grilles de Sudoku
 * \author Simon Manchel & Woody Rousseau
 * \version 0.1
 * \date 19 avril 2013
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <sys/timeb.h>

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
	// L'utilisateur demande de l'aide pour l'utilisation du programme
	if ( ( argc >= 2) && (strcmp ( argv[1] , "help" ) == 0) ) 
	{
		displayUsage();
		exit ( EXIT_SUCCESS );
	}

	// Démarrage de l'horloge pour mesurer le temps d'exécution
	struct timeb timeTmp;
	ftime(&timeTmp);
	double timeStart, timeEnd;
	double time_spent;
	timeStart = ( (double) timeTmp.time) + ( (double) timeTmp.millitm/1000);
	//gettimeofday( &timeStart, NULL);

	// Déclaration de la grille et de variables dont il faudra allouer la mémoire
	unsigned char** grid; 
	unsigned char blocksPerSquare;
	subGrid* threads = NULL;

	// Utilisation des arguments de path d'entrée-sortie (avec cas par défault)
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

	blocksPerSquare = readDimensions(filePath); // On obtient les dimensions de la grille
	createGrid(blocksPerSquare, &grid); // On alloue la mémoire pour la grille
	readGrid(filePath, grid, blocksPerSquare); // On remplit la grille avec le fichier texte d'entrée
	initSudoku( grid, blocksPerSquare); // On alloue la mémoire du Sudoku (variable globale)
	launchThreads( &threads ); // Lancement des threads
	writeGrid(resultPath, grid, blocksPerSquare); // On écrit dans le fichier de sortie
	deleteSudoku(sudoku); // On désalloue la mémoire du Sudoku

	// Fin de l'horloge pour le temps d'éxécution
	ftime(&timeTmp);
	timeEnd = ( (double) timeTmp.time) + ( (double) timeTmp.millitm/1000);
	time_spent = timeEnd - timeStart;
	
	// Ecriture des statistiques dans le fichier ThreadsStats.txt
	printStatsThread(NULL, time_spent);
	printf("Temps d'Exécution : %0.3f secondes\n" ,time_spent);

	exit( EXIT_SUCCESS );
}

void displayUsage()
{
	fprintf(stdout , "Usage :\n");
	fprintf(stdout, "\"./exe/sudoku\" / \"make run\" : Default avec \"sudoku.in.txt\" en entrée et sudoku.out.txt en sortie.\n");
	fprintf(stdout, "\"./exe/sudoku [INPUT]\" : Grille d\'entrée spécifiée.\n");
	fprintf(stdout, "\"./exe/sudoku [INPUT] [OUTPUT]\" : Grilles d\'entrée et de sortie spécifiées.\n");
	
	exit ( EXIT_SUCCESS );
}
