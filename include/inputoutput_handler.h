/**
 * \file inputoutput_handler.h
 * \brief Header des fonctions gérant les entrées et sorties du programme
 * \author Simon Manchel & Woody Rousseau
 * \version 0.1
 * \date 19 avril 2013
 */

#ifndef D_INPUTOUTPUT_HANDLER
#define D_INPUTOUTPUT_HANDLER

/**
 * \fn unsigned char readDimensions(char* filePath)
 * \brief: Donne les dimensions du Sudoku
 * \param[in]: filePath Chemin relatif du fichier texte de la grille
 * \result: Un unsigned char (meilleures performances) donnant les dimensions de la grille 
 */
unsigned char readDimensions(char* filePath);

/**
 * \fn void readGrid(char* filePath, unsigned char** grid, unsigned char dim)
 * \brief: Traite le fichier texte pour obtenir un tableau 2D pour la grille
 * \param[in]: filePath Chemin relatif du fichier texte de la grille
 * \param[out]: grid Tableau 2D représentant la grille
 * \param: dim Dimensions de la grille
 */
void readGrid(char* filePath, unsigned char** grid, unsigned char dim);

/**
 * \fn void writeGrid(char* resultPath, unsigned char** grid, unsigned char dim)
 * \brief: Ecrit la grille résolue dans un fichier texte
 * \param[out]: resultPath Chemin relatif du fichier texte de la grille résolue
 * \param[in]: grid Tableau 2D représentant la grille
 * \param: dim Dimensions de la grille
 */
void writeGrid(char* resultPath, unsigned char** grid, unsigned char dim);

/**
 * \fn void printGrid()
 * \brief: Fonction de Debug affichant la grille sur stdout
 */
void printGrid();

/**
 * \fn printStatsThread(threadParameters* parameters, double time_spent)
 * \brief: Imprime les Stats des threads et le temps d'execution dans le fichier ThreadsStats.txt
 * \param: parameters Pointeur vers les paramètres du thread
 * \param: time_spent Temps pris pour l'éxécution du programme jusqu'à ce point
 */
void printStatsThread(threadParameters* parameters, double time_spent);

#endif
