#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "memory_handler.h"

void *threadStart(void* arg)
{
	parThread* tab = arg;
	int n = tab->n, i = tab->i;
	//printf("%d\n", (int) tab[1]);
	initSubGrid( tab->subGrid , i, n );


	return NULL;
}