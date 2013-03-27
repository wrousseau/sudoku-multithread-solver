#ifndef D_INPUTOUTPUT_HANDLER
#define D_INPUTOUTPUT_HANDLER

unsigned char readDimensions(char* filePath);//retourne la longueur du coté de la grille
void readGrid(char* filePath, unsigned char** grid, unsigned char n);//lit un fichier texte et transmet les informations dans la matrice grid

#endif
