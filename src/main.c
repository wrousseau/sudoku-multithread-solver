#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "memory_handler.h"
#include "inputoutput_handler.h"
#include "solver.h"

int main (int argc, char **argv)
{	
	unsigned char** grid; //déclaration du double pointeur de la matrice représentant la grille de jeu
	unsigned char blocksPerSquare;
	char defaultFilePath[] = "sudoku_in.txt", defaultResultPath[] = "sudoku_out.txt";
	char *filePath = defaultFilePath, *resultPath = defaultResultPath;
	if(argc > 1)
	{
		filePath = argv[1];
	}
	if(argc > 2)
	{
		resultPath = argv[2];
	}
	blocksPerSquare = readDimensions(filePath);
	createGrid(blocksPerSquare, &grid );
	readGrid(filePath, grid, blocksPerSquare);
	
	
	writeGrid(resultPath, grid, blocksPerSquare);
	deleteGrid( blockPerSquare, grid);
	exit( EXIT_SUCCESS );
}
