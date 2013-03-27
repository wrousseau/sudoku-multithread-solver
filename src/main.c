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
	char defaultFilePath[] = "sudoku_in.txt";
	char *filePath = defaultFilePath;
	if(argc > 1)
	{
		filePath = argv[1];
	}
	blocksPerSquare = readDimensions(filePath);
	createGrid(blocksPerSquare, &grid );
	readGrid(filePath, grid, blocksPerSquare);
	
	exit( EXIT_SUCCESS );
}
