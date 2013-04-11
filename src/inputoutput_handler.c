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

	int i = 0, j, size = 255*4, number;
	char buf[size];
	char *tmp;

	fgets( buf, size, file ); // On passe la première ligne

	while(fgets(buf, size, file) != NULL)
	{
		for(tmp = strtok(buf," \t\n"), j = 0;
			tmp != NULL;
			tmp = strtok(NULL, " \t\n") , j++ ) // On parcours chaques nombres séparés par un espace.
		{
			number = atoi(tmp);
			if(number > 255)
			{
				perror ( "Un élément est supérieur à 255 dans la table fournie" );
				exit ( EXIT_FAILURE );
			}
			grid[i][j] = (unsigned char) number;
		}
		i++;
	}

	fclose(file);

	return;	
}

void writeGrid(char* resultPath, unsigned char** grid, unsigned char n)
{
	FILE* file = fopen(resultPath, "w");
	if(file == NULL)
	{
		perror( "Problème lors de l'ouverture en écriture du fichier de sortie." );
		exit( EXIT_FAILURE );
	}

	fprintf(file, "%d\n", n); //on imprime la taille du sudoku
	
	for( int i = 0 ; i < n ; i++)
	{
		for( int j = 0 ; j < n-1 ; j++)
		{
			fprintf(file, "%d ", grid[i][j]);
			// Si on est pas en fin de ligne, on rajoute un espace entre 2 nombres
		}
		fprintf(file, "%d\n", grid[i][n-1]);
	}

	fclose(file);
	
	return;
}




void printGrid()
{
	printf("GRID empty:%d\n", sudoku->emptyBlocks);
	for( int i = 0 ; i < sudoku->blocksPerSquare ; i++)
	{
		for( int j = 0 ; j < sudoku->blocksPerSquare ; j++)
		{
			printf("%d ",sudoku->grid[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return;
}



void printStatsThread(threadParameters* par, double time_spent)
{
	FILE* file;
	if(time_spent == 0) // écriture des données sur les threads
	{
		if(par->threadNumber == 0) //Pour la première série, on efface l'ancien fichier et on met l'en tête
		{
			file = fopen("ThreadsStats.txt", "w");
			if(file == NULL)
			{
				perror("Erreur lors de l'écriture des stats : ");
				exit( EXIT_FAILURE );
			}
			fprintf(file, "###### STATISTIQUES DES THREADS ######\n");
			fprintf(file, "%d Threads\n\n\n", par->numberOfBlocks );
		}
		else // sinon on ne fait qu'ajouter les données -> append
		{
			file = fopen("ThreadsStats.txt", "a");
			if(file == NULL)
			{
				perror("Erreur lors de l'écriture des stats : ");
				exit( EXIT_FAILURE );
			}
		}		

		fprintf(file, "Thread %d :\n -------------------\n", par->threadNumber);
		fprintf(file, "Nombre de cases du carré secondaire restant à résoudre au démarrage : %d\n", par->subGrid->emptyAtBoot );
		fprintf(file, "Nombre total de solutions à explorer au démarrage : %d\n", par->subGrid->solAtBoot);
		fprintf(file, "Nombre d’exécutions : %d\n", par->subGrid->numberLaunch);
		fprintf(file, "Nombre d’exécutions utiles : %d\n", par->subGrid->successLaunch);
		fprintf(file, "Nombre d’exécutions inutiles : %d\n\n\n", par->subGrid->failLaunch);
	}
	else // écriture du temps d'execution à la fin du fichier
	{
		file = fopen("ThreadsStats.txt", "a");
		if(file == NULL)
		{
			perror("Erreur lors de l'écriture des stats : ");
			exit( EXIT_FAILURE );
		}
		fprintf(file, "Temps d'Exécution : %f secondes\n" ,time_spent);
	}

	fclose(file);
	return;
}
















