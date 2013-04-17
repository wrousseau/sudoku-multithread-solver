/**
 * \file solver.h
 * \brief Header des fonctions gérant la résolution du Sudoku et exécutées sur chaque thread
 * \author Simon Manchel & Woody Rousseau
 * \version 0.1
 * \date 13 avril 2013
 */

#ifndef D_SOLVER
#define D_SOLVER

/**
 * \fn void *threadStart(void* arg)
 * \brief: Mise en place du thread et lancement
 * \param: arg Argument envoyé au thread par pthread_create
 */
void *threadStart(void* arg);

/**
 * \fn void searchChoices(unsigned char** result, subGrid* currentSubGrid, int numberOfBlocks, int subSquareWidth)
 * \brief: renvoie les points trouvés sous la forme X*[valeur,i,j][0...]
 * \param: result 
 * \param: gridAdress Un pointeur vers la grille
 */
void searchChoices(unsigned char **result , subGrid* currentSubGrid, int numberOfBlocks, int subSquareWidth);

/**
 * \fn void initChoices(subGrid* currentSubGrid, int widthSubSquare)
 * \brief: Alloue la mémoire pour la grille (tableau 2D)
 * \param: size La dimension de la grille
 * \param: gridAdress Un pointeur vers la grille
 */
void initChoices(subGrid* currentSubGrid, int widthSubSquare);

/**
 * \fn void fillGrid(unsigned char* result, threadParameters* tab)
 * \brief: Alloue la mémoire pour la grille (tableau 2D)
 * \param: size La dimension de la grille
 * \param: gridAdress Un pointeur vers la grille
 */
void fillGrid(unsigned char* result, threadParameters* tab);

/**
 * \fn unsigned char checkBlock(Solution *s, subGrid* thread, unsigned char i, unsigned char j)
 * \brief: Alloue la mémoire pour la grille (tableau 2D)
 * \param: size La dimension de la grille
 * \param: gridAdress Un pointeur vers la grille
 */
unsigned char checkBlock(Solution *s, subGrid* thread, unsigned char i, unsigned char j);

/**
 * \fn unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j)
 * \brief: Alloue la mémoire pour la grille (tableau 2D)
 * \param: size La dimension de la grille
 * \param: gridAdress Un pointeur vers la grille
 */
unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j);

/**
 * \fn unsigned char getSingletonChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j)
 * \brief: Alloue la mémoire pour la grille (tableau 2D)
 * \param: size La dimension de la grille
 * \param: gridAdress Un pointeur vers la grille
 */
unsigned char getSingletonChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j);



#endif
