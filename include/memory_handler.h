/**
 * \file memory_handler.h
 * \brief Header des fonctions gérant la mémoire dans le programme (allocations et libération de mémoire)
 * \author Simon Manchel & Woody Rousseau
 * \version 0.1
 * \date 13 avril 2013
 */

#ifndef D_MEMORY_HANDLER
#define D_MEMORY_HANDLER

/**
 * \fn void createGrid(unsigned char size, unsigned char*** gridAdress )
 * \brief: Alloue la mémoire pour la grille (tableau 2D)
 * \param: size La dimension de la grille
 * \param: gridAdress Un pointeur vers la grille
 */
void createGrid(unsigned char size, unsigned char*** gridAdress );

/**
 * \fn void deleteSudoku()
 * \brief: Désalloue la mémoire du Sudoku (Variable Globale)
 */
void deleteSudoku();

/**
 * \fn initSudoku(unsigned char** grid, unsigned char blocksPerSquare)
 * \brief: Alloue la mémoire pour le Sudoku (Variable Globale)
 * \param: grid Tableau 2D représentant la grille
 * \param: size Dimensions de la grille
 */
void initSudoku( unsigned char** grid, unsigned char blocksPerSquare);

/**
 * \fn launchThreads( subGrid** subGridsAdresses, int blocksPerSquare)
 * \brief: Lance les différents threads
 * \param: subGridsAdresses Double pointeur vers les sous-grilles
 * \param: blocksPerSquare Nombre de blocs par sous-carré
 */
void launchThreads( subGrid** subGridsAdresses );

/**
 * \fn void initSubGrid( subGrid* subGrid , int threadNumber, int numberOfBlocks )
 * \brief: Alloue la mémoire pour une sous-grille
 * \param: subGrid Pointeur vers la sous-grille à allouer
 * \param: threadNumber Numéro du thread considéré
 * \param: numberOfBlocks Nombre de cases par sous-grille
 */
void initSubGrid( subGrid* subGrid , int threadNumber );

/**
 * \fn void initResult(unsigned char **result, int numberOfBlocks)
 * \brief: Alloue la mémoire pour les résultats
 * \param: result Double pointeur vers les résultats (chaînes de caractères)
 * \param: numberOfBlocks Nombre de cases par sous-grille
 */
void initResult(unsigned char **result );

void cleanSubGrids( threadParameters* parameters);


#endif
