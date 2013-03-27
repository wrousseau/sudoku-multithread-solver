#ifndef D_MEMORY_HANDLER
#define D_MEMORY_HANDLER



unsigned char** createGrid(unsigned char n, unsigned char*** gridAdress );//n : nombre de lignes (ou de colonnes selon le point de vue). gridAdress : Adresse du double pointeur de la grille

void deleteGrid(unsigned char n, block** grid);


#endif
