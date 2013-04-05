#ifndef D_SOLVER
#define D_SOLVER

void *threadStart(void* arg);
unsigned char* searchChoices(subGrid* par, int n, int widthSubSquare, bool init);// renvoie les points trouvés sous la forme X*[valeur,i,j][0...]
void fillGrid(unsigned char* result, parThread* tab);
unsigned char checkBlock(Solution *s, subGrid* thread, unsigned char i, unsigned char j, bool init);
unsigned char getNaiveChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j);
unsigned char getSingletonChoices(Solution *s, subGrid* thread, unsigned char i, unsigned char j);


#endif
