#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "mem_work.h"

int main (int argc, char **argv)
{
	block** grid; //déclaration du double pointeur de la matrice représentant la grille de jeu
	int blockPerSquare = 9;
	createGrid(blockPerSquare, &grid );
	
	exit(EXIT_SUCCESS);
}
