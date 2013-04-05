#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structures.h"

extern Sudoku* sudoku;

unsigned char readDimensions( char *filePath )
{
	FILE* file = fopen( filePath , "r" );
	if ( file == NULL )
	{
		perror ( "Erreur lors de la lecture du Sudoku d'entrée." );
		exit ( EXIT_FAILURE );
	}
	
	int dim; // On passe par un int en premier lieu pour voir si ça dépasse 255
	char buf[10]; // 10 chiffres suffisent à priori
	fgets ( buf , 10 , file );
	buf[strlen(buf)-1] = '\0'; // Suppression du retour chariot
	dim = atoi ( buf );
	if ( dim > 255 )
	{
		perror ( "Erreur : Le programme ne peut gérer de sudoku dont la dimension dépasse 255" );
		exit ( EXIT_FAILURE );
	}

	fclose ( file );
	
	return ( unsigned char ) dim ; // On recaste en "unsigned char" pour des soucis de mémoire
}

void readGrid ( char* filePath , unsigned char** grid , unsigned char n )
{
	FILE* file = fopen ( filePath , "r" );
	if( file == NULL )
	{
		perror ( "Erreur lors de la lecture du Sudoku d'entrée." );
		exit( EXIT_FAILURE );
	}

	int tmp, j, k, p, size = 255*4;
	char buf[size];
	char number[10];
	fgets( buf, size, file ); // On passe la première ligne

	// Lecture des lignes
	for ( int i = 0 ; i < n ; i++ ) 
	{
		j = 0;
		k = 0;
		p = 0;

		if (fgets(buf, size, file) == NULL)
    	{
        	perror ( "EOF Précoce" );
        	exit ( EXIT_FAILURE );
    	}

		if (buf[strlen(buf) - 1] == '\n') 
		{
  			buf[strlen(buf) - 1] = '\0';
		}

		// Lecture des colonnes de la ligne
		while(buf[p] != '\0' && j!=n) 
		{
			// Tant qu'il y a des chiffres on est sur un nombre, on le récupère dans un Sous-Buffer (number)
			while(buf[p] != ' ' && buf[p] != '\0') 
			{
				number[k] = buf[p];
				k++;
				p++;
			}
			number[k] = '\0';
			k = 0;
			tmp = atoi( number );
			if(tmp > 255)
			{
				perror("Un élément est supérieur à 255 dans la table fournie");
				exit( EXIT_FAILURE );
			}
			grid[i][j] = ( unsigned char ) tmp;
			j++;
			p++;
		}
	}

	fclose(file);

	return;	
}

void writeGrid(char* resultPath, unsigned char** grid, unsigned char n)
{
	int i, j;
	FILE* file = fopen(resultPath, "w");
	if(file == NULL)
	{
		perror( "Problème lors de l'ouverture en écriture du fichier de sortie." );
		exit( EXIT_FAILURE );
	}
	fprintf(file, "%d\n", n); //on imprime la taille du sudoku
	
	for(i = 0 ; i < n ; i++)
	{
		for( j = 0 ; j < n-1 ; j++)
		{
			fprintf(file, "%d ", grid[i][j]);
			// Si on est pas en fin de ligne, on rajoute un espace entre 2 nombres
		}
		fprintf(file, "%d\n", grid[i][j]);
	}
	
	fclose(file);
	
	return;
}




void printGrid()
{
	printf("GRID empty:%d\n", sudoku->emptyBlocks);
	for( int i = 0 ; i < sudoku->n ; i++)
	{
		for( int j = 0 ; j < sudoku->n ; j++)
		{
			printf("%d ",sudoku->grid[i][j]);
		}
		printf("\n");
	}printf("\n");
	return;
}
















