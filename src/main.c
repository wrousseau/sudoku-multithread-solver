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
	if(argc < 2)
	{
		blocksPerSquare = readDimensions("sudoku_in.txt");
	}
	else
	{
		blocksPerSquare = readDimensions(argv[1]);
	}
	createGrid(blocksPerSquare, &grid );
	
	exit( EXIT_SUCCESS );
}
