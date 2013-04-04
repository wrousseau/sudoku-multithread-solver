#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "structures.h"
#include "memory_handler.h"
#include "inputoutput_handler.h"
#include "solver.h"

Sudoku* sudoku;

int main (int argc, char **argv)
{	
	unsigned char** grid; // Déclaration du double pointeur de la matrice représentant la grille de jeu
	
	unsigned char blocksPerSquare;
	char *filePath = "sudoku_in.txt", *resultPath = "sudoku_out.txt";
	subGrid* threads=NULL;

	if(argc > 1)
	{
		filePath = argv[1];
	}
	if(argc > 2)
	{
		resultPath = argv[2];
	}

	blocksPerSquare = readDimensions(filePath);
	createGrid(blocksPerSquare, &grid);
	readGrid(filePath, grid, blocksPerSquare);
	initSudoku( &sudoku, grid, blocksPerSquare);
	launchThreads( &threads, blocksPerSquare);
	
	writeGrid(resultPath, grid, blocksPerSquare);
	deleteSudoku(sudoku);
	
	exit( EXIT_SUCCESS );
}
