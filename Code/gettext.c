#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "det.h"

int
main(int argc, char** argv)
{
	int length, i;

	if (argc != 5)
		my_err_quit("usage: gettext -a <Text_length> -o <filename>");
	
	if ((length = (int)strtol(argv[2],NULL,10)) <= 1)
		my_err_quit("<Text_length> must be a number bigger than 1");
	
	FILE *fp = NULL;
	if((fp = fopen(argv[4],"w")) == NULL)
		perror("fopen error"),exit(1);

	srand((unsigned int)time(NULL));
	for (i = 0; i < length; ++i)
	{
		fprintf(fp, "%c", (char)(rand() % MOD + (int)('A')));
	}

	fclose(fp);

	return 0;
}
