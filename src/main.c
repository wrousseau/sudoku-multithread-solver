#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "structures.h"
#include "memory_handler.h"
#include "inputoutput_handler.h"
#include "solver.h"


Sudoku* sudoku;

int main (int argc, char **argv)
{	
	clock_t begin, end;
	double time_spent;
	begin = clock();
	unsigned char** grid; // Déclaration du double pointeur de la matrice représentant la grille de jeu
	
	unsigned char blocksPerSquare;
	char *filePath = "sudoku_in.txt", *resultPath = "sudoku_out.txt";
	subGrid* threads = NULL;

	if(argc > 1)
	{
		filePath = argv[1];
	}
	if(argc > 2)
	{
		resultPath = argv[2];
	}

	size = readDimensions(filePath);
	createGrid(blocksPerSquare, &grid);
	readGrid(filePath, grid, blocksPerSquare);
	initSudoku( &sudoku, grid, blocksPerSquare);
	launchThreads( &threads, blocksPerSquare);
	
	writeGrid(resultPath, grid, blocksPerSquare);
	deleteSudoku(sudoku);
	end = clock();
	time_spent = (double) (end - begin) / CLOCKS_PER_SEC; 
	printf("Temps d'Exécution : %f seconds\n" ,time_spent);
	exit( EXIT_SUCCESS );
}
