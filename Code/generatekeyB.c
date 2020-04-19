#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "det.h"

int
main(int argc, char** argv)
{
	int dimen, i, r;

	if (argc != 5)
		my_err_quit("usage: generatekeyB -c <Dimension>");

	if ((dimen = (int)strtol(argv[2],NULL,10)) <= 0)
		my_err_quit("<Dimension> must be a number bigger than 0");

	int* Array = (int *)malloc(sizeof(int) * dimen);
	
	srand((unsigned int)time(NULL));

	for(i = 0; i < dimen - 1; ++i){
		r = rand() % MOD;
		Array[i] = r;
		printf("%d\t", r);
	}
	r = rand() % MOD;
	Array[dimen - 1] = r;
	printf("%d\n", r);

	write_array(argv[4], Array, dimen);

	return 0;
}
