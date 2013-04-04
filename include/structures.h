#ifndef D_STRUCTURES
#define D_STRUCTURES

typedef struct{
	unsigned char** grid;
	int emptyBlocks;
	unsigned char n;
}Sudoku;

typedef struct{
	int N_sol;
	unsigned char *p;
}Solution;

typedef struct{
	int N;
	Solution** s;
}subGrid;


#endif
