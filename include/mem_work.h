#ifndef MEM_WORK
#define MEM_WORK



block** createGrid(unsigned char n, block*** gridAdress );//n : nombre de lignes (ou de colonnes selon le point de vue). gridAdress : Adresse du double pointeur de la grille

void deleteGrid(unsigned char n, block** grid);


#endif
