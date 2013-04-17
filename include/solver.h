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
 * \fn void searchChoices(unsigned char** result, subGrid* currentSubGrid)
 * \brief: Lance les calculs sur tout le sous carré concerné par le threads, et stocke les résultats du cycle dans result.
 * \param: result est un tableau de int. Il est regroupé en bloc de 3 int : valeur, i, j (i et j étant les coordonnées matricielles dans la grille-Matrice). La fin du tableau est détectée dès qu'une valeur est à 0
 * \param: currentSubGrid Pointeur vers la structure subGrid utilisée par le thread
 */
void searchChoices(unsigned char **result , subGrid* currentSubGrid);

/**
 * \fn void initChoices(subGrid* currentSubGrid)
 * \brief: Initialise les choix du sous carré, afin de détecter ceux que l'on connait, et d'initialiser les choix restants des autres
 * \param: currentSubGrid Pointeur vers la structure subGrid utilisée par le thread
 */
void initChoices(subGrid* currentSubGrid);

/**
 * \fn void fillGrid(unsigned char* result, threadParameters* tab)
 * \brief: Rempli la grille à l'aide du tableau result 
 * \param: result Tableau de int contenant les résultat sous la forme : valeur, i, j, valeur, i , j, ... , 0, ...
 * \param: tab Pointeur vers les paramètres du thread contenants la subGrid et le numéro du thread
 */
void fillGrid(unsigned char* result, threadParameters* tab);

/**
 * \fn unsigned char checkBlock(Solution *s, subGrid* thread, unsigned char yGlobal, unsigned char xGlobal)
 * \brief: lance les calculs pour un bloc du sous carré
 * \param: s Pointeur vers la structure Solution représentant le bloc
 * \param: currentSubGrid Pointeur vers la subGrid utilisée par le pointeur
 * \param: yGlobal Ordonée du bloc concerné
 * \param: xGlobal Abcisses du bloc concerné
 * \return: 0 si on a rien trouvé, le nombre si on l'a trouvé
 */
unsigned char checkBlock(Solution *s, subGrid* currentSubGrid, unsigned char yGlobal, unsigned char xGlobal);

/**
 * \fn unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char yGlobal, unsigned char xGlobal)
 * \brief: Recherche les solutions naïves pour un bloc : c'est à  dire quand il y a un seul choix possible dans un bloc
 * \param: s Pointeur vers la structure Solution représentant le bloc
 * \param: currentSubGrid Pointeur vers la subGrid utilisée par le pointeur
 * \param: yGlobal Ordonée du bloc concerné
 * \param: xGlobal Abcisses du bloc concerné
 * \return: 0 si on a rien trouvé, le nombre si on l'a trouvé
 */
unsigned char getNaiveChoices(Solution *s, subGrid* currentSubGrid, unsigned char yGlobal, unsigned char xGlobal);

/**
 * \fn unsigned char getSingletonChoices(Solution *s, subGrid* thread, unsigned char yGlobal, unsigned char xGlobal)
 * \brief: Recherche les solutions uniques : c'est à dire que si un nombre ne peut se trouver que dans une case du sous carré, elle doit aller dans cette case
 * \param: s Pointeur vers la structure Solution représentant le bloc
 * \param: currentSubGrid Pointeur vers la subGrid utilisée par le pointeur
 * \param: yGlobal Ordonée du bloc concerné
 * \param: xGlobal Abcisses du bloc concerné
 * \return: 0 si on a rien trouvé, le nombre si on l'a trouvé
 */
unsigned char getSingletonChoices(Solution *s, subGrid* currentSubGrid, unsigned char yGlobal, unsigned char xGlobal);

/**
 * \fn void countSolution(subGrid* currentSubGrid)
 * \brief: Compte le nombre de solution au démarrage
 * \param: currentSubGrid Pointeur vers la subGrid utilisée par le pointeur
 */
void countSolution(subGrid* currentSubGrid);

#endif
