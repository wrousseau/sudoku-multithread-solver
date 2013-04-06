#ifndef D_MEMORY_HANDLER
#define D_MEMORY_HANDLER

void createGrid(unsigned char size, unsigned char*** gridAdress );//n : nombre de lignes (ou de colonnes selon le point de vue). gridAdress : Adresse du double pointeur de la grille
void deleteSudoku(Sudoku* sudoku);
void initSudoku( Sudoku** sudokuAdress, unsigned char** grid, unsigned char size);
void launchThreads( subGrid** threadsAdress, int size);
void initSubGrid( subGrid* par , int i, int n );

#endif
