#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "structures.h"
#include "memory_handler.h"
#include "inputoutput_handler.h"
#include "solver.h"


Sudoku* sudoku;

void displayUsage();

int main (int argc, char **argv)
{
	if ( ( argc >= 2) && strcmp ( argv[1] , "help\n" ) ) 
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
	initSudoku( &sudoku, grid, blocksPerSquare);
	launchThreads( &threads, blocksPerSquare);
	
	writeGrid(resultPath, grid, blocksPerSquare);
	deleteSudoku(sudoku);

	gettimeofday( &timeEnd, NULL);
	time_spent = ((double) (timeEnd.tv_usec - timeStart.tv_usec)) / 1000000;
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
