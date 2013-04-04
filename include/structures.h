#ifndef D_STRUCTURES
#define D_STRUCTURES

#include <stdbool.h>
#include <pthread.h>

typedef struct{
	unsigned char** grid;
	int emptyBlocks;
	unsigned char n;
	bool locked;
}Sudoku;

typedef struct{
	int N_sol;
	unsigned char choices[255];
}Solution;

typedef struct{
	pthread_t thread;
	int emptyBlocks;
	int solBoot;
	int numberLaunch;
	int successLaunch;
	int failLaunch;
	unsigned char x;
	unsigned char y;
	Solution** s;
}subGrid;

typedef struct{
	int i;
	int n;
	subGrid* subGrid;
}parThread;


#endif
