#include <stdlib.h>
#include <stdio.h>
#include <string.h>	

unsigned char readDimensions(char* filePath)
{
	FILE* file = fopen(filePath, "r");
	if(file == NULL)
	{
		perror("Erreur lors de la lecture du Sudoku d'entrée ");
		exit( EXIT_FAILURE);
	}
	
	int tmp;
	char buf[100];
	fgets(buf, 100, file);
	buf[strlen(buf)-1] = '\0'; //Pour supprimer le \n de fgets
	tmp = atoi(buf);
	if(tmp > 255)
	{
		printf("Erreur : Le programme ne peut gérer de sudoku dont la dimension dépasse 255\n");
		exit( EXIT_FAILURE );
	}
	fclose(file);
	
	return (unsigned char) tmp;
}

/////////////////////////////////

void readGrid(char* filePath, unsigned char** grid, unsigned char n)
{
	FILE* file = fopen(filePath, "r");
	if(file == NULL)
	{
		perror("Erreur lors de la lecture du Sudoku d'entrée ");
		exit( EXIT_FAILURE);
	}
	int tmp, i, j, k, p, size = 255*4;
	char buf[size];
	char minibuf[10];
	fgets(buf, size, file); //On passe la première ligne
	//lecture des lignes
	for(i = 0 ; i < n ; i++) 
	{
		j = 0;
		k = 0;
		p = 0;
		fgets(buf, size, file);
		buf[strlen(buf)-1] = '\0'; //Pour supprimer le \n de fgets
		
		//lecture des colonnes de la ligne (des chiffres)
		while(buf[p] != '\0') 
		{
			
			if(buf[p] != ' ') //Tant qu'il y a des chiffres on est sur un nombre, on le récupère dans un 2° buffer
			{
				minibuf[k] = buf[p];
				k++;
			}
			else // On a fini de lire un nombre, on le transforme en unsigned char et on le stocke dans notre matrice
			{
				minibuf[k] = '\0'; //on définit la fin de string
				tmp = atoi(minibuf);
				if(tmp > 255)
				{
					printf("Erreur : un élément est supérieur à 255 dans la table fournie\n");
					exit( EXIT_FAILURE );
				}
				grid[i][j] = (unsigned char) tmp; //on stocke dans la matrice
				j++; //on prépare l'indice pour la prochaine écriture
				k = 0; //on réinitialise k pour le buffer des nombres suivants
			}
		p++;
		}
		//on récupère le dernier nombre avant le \0 de la ligne si ce dernier est collé au dernier nombre
		if(j == n-1)
		{
			minibuf[k] = '\0';
			tmp = atoi(minibuf);
			if(tmp > 255)
			{
				printf("Erreur : un élément est supérieur à 255 dans la table fournie\n");
				exit( EXIT_FAILURE );
			}
			grid[i][j] = (unsigned char) tmp;
		}
	}
	
	fclose(file);
	
	return;	
}


///////////////////////////////////////////////////:

void writeGrid(char* resultPath, unsigned char** grid, unsigned char n)
{
	int i, j;
	FILE* file = fopen(resultPath, "w");
	if(file == NULL)
	{
		perror("Problème lors de l'ouverture en écriture du fichier de sortie ");
		exit( EXIT_FAILURE );
	}
	fprintf(file, "%d\n", n); //on imprime la taille du sudoku
	
	for(i = 0 ; i < n ; i++)
	{
		for( j = 0 ; j < n ; j++)
		{
			fprintf(file, "%d", grid[i][j]);
			// Si on est pas en fin de ligne, on rajoute un espace entre 2 nombres
			if(j < n-1)
			{
				fputc( ' ', file);
			}
		}
		fputc( '\n', file); // retour à la ligne entre chaque ligne
	}
	fclose(file);
	
	return;
}





















