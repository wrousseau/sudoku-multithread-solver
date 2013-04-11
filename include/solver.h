#ifndef D_SOLVER
#define D_SOLVER

void *threadStart(void* arg);
void searchChoices(unsigned char** result, subGrid* par, int n, int widthSubSquare);// renvoie les points trouvés sous la forme X*[valeur,i,j][0...]
void initChoices(subGrid* currentSubGrid, int widthSubSquare);
void fillGrid(unsigned char* result, threadParameters* tab);
unsigned char checkBlock(Solution *s, subGrid* thread, unsigned char i, unsigned char j);
unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j);
unsigned char getSingletonChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j);


#endif
