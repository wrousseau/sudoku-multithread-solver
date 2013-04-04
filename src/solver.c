#include <stdlib.h>
#include <stdio.h>
#include "structures.h"

void threadStart(void* arg)
{
	parThread* tab = arg;
	int n = tab->n, i = tab->i;
	printf("%d\n", tab[1]);
	initSubGrid( tab->subGrid , i, n );


	return;
}