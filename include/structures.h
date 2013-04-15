/**	
 * \file structures.h
 * \brief Header des structures utilisées dans le programme
 * \author Simon Manchel & Woody Rousseau
 * \version 0.1
 * \date 13 avril 2013
 */

#ifndef D_STRUCTURES
#define D_STRUCTURES

#include <stdbool.h>
#include <pthread.h>

/**
 * \struct Sudoku
 * \brief Objet contenant la grille et ses caractéristiques principales
 */
typedef struct{
	unsigned char** grid; /*!< Grille (Tableau 2D) à traiter */
	int emptyBlocks; /*!< Nombre de cases non résolues */
	unsigned char blocksPerSquare; /*!< Nombre de cases par carré */
	bool locked; /*!< Indique si la grille est verouillée */
} Sudoku;

/**
 * \struct Solution
 * \brief Objet contenant les différentes propositions pour une case
 */
typedef struct{
	int N_sol; /*!< Nombre de propositions à envisager pour la case considérée. */
	unsigned char choices[255]; /*!< Différentes propositions pour la case considérée. */
} Solution;

/**
 * \struct subGrid
 * \brief Objet correspondant à chaque sous-carré (sous-grille)
 *
 * subGrid contient notamment le thread correspondant au carré
 */
typedef struct{
	pthread_t thread; /*!< Thread correspondant au sous-carré*/
	int emptyBlocks; /*!< Nombre de cases vides dans la sous-grille */
	int solAtBoot; /*!< Nombre de solutions à explorer au démarrage */
	int emptyAtBoot; /*!< Nombre de cases du sous-carré restant à résoudre au démarrage */
	int numberLaunch; /*!< Nombre d'éxécutions */
	int successLaunch; /*!< Nombre d'éxécutions utiles */
	int failLaunch; /*!< Nombre d'éxécutions inutiles */
	unsigned char x; /*!< Position "Horizontale" du sous-carré */
	unsigned char y; /*!< Position "Verticale" du sous-carré */
	Solution** solution; /*!< Double pointeur vers les solutions des différentes cases */
} subGrid;

/**
 * \struct threadParameters
 * \brief Objet comprenant les paramètres des threads lancés
 */
typedef struct {
	int threadNumber; /*!< Numéro du Thread */
	int numberOfBlocks; /*!< Nombre de cases traitées par le thread */
	subGrid* subGrid; /*!< Sous-Grille du Thread */
} threadParameters;

#endif
