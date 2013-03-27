#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "mem_work.h"
#include "streaminout.h"

int main (int argc, char **argv)
{	
	block** grid; //déclaration du double pointeur de la matrice représentant la grille de jeu
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
	
	exit(EXIT_SUCCESS);
}
