#ifndef D_SOLVER
#define D_SOLVER

void *threadStart(void* arg);
unsigned char* searchChoices(subGrid* par, int n, int widthSubSquare);// renvoie les points trouvés sous la forme X*[valeur,i,j][0...]

#endif
