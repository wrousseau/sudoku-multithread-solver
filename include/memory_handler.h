#ifndef D_MEMORY_HANDLER
#define D_MEMORY_HANDLER



void createGrid(unsigned char n, unsigned char** grid );//n : nombre de lignes (ou de colonnes selon le point de vue). gridAdress : Adresse du double pointeur de la grille

void deleteSudoku(Sudoku* sudoku);
void initSudoku( Sudoku** sudokuAdress, unsigned char*** gridAdress, unsigned char npar);


#endif
