#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "det.h"

void decrypt(int* X, int** A_1, int* B, int n, FILE* fp);

int
main(int argc, char** argv)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);

	int dimen, i, ch, count;

	if (argc != 9)
		my_err_quit("usage: decrypt -c <Dimension> -i <Ciphertext File> <Key A_1 File> <Key B File> -o <Output File>");

	if ((dimen = (int)strtol(argv[2],NULL,10)) <= 1)
		my_err_quit("<Dimension> must be a number bigger than 1");

	int** A_1 = (int**)malloc(dimen * sizeof(int *));
	for (i = 0; i < dimen; ++i)
		A_1[i] = (int*)malloc(dimen * sizeof(int));
	
	read_matrix(argv[5], A_1, dimen);

	int* B = (int *)malloc(dimen * sizeof(int *));
	
	read_array(argv[6], B, dimen);
	
	// 清空目标文件内容
	FILE *pfp0 = fopen(argv[8],"w");
	fclose(pfp0);
	
	FILE *pfp = NULL;
	if ((pfp = fopen(argv[8],"a")) == NULL)
		perror("fopen error"), exit(1);

	FILE *cfp = NULL;
	if ((cfp = fopen(argv[4],"r")) == NULL)
		perror("fopen error"), exit(1);
	
	int* X = (int *)malloc(dimen * sizeof(int *));

	count = 0;
	while(1){
		ch = fgetc(cfp);
		if (ch >= 65 && ch <= 90)
			X[count] = ch - 65;
		else if (ch >= 97 && ch <= 122)
			X[count] = ch - 97;
		else if (ch == '\n' || ch == '\r')
			continue;
		else if (ch == EOF)
			break;
		else
			my_err_quit("illegal text");
		
		count++;
		if (count == dimen){
			count = 0;
		/*	for (i = 0; i < dimen; ++i){
				printf("%d\t", X[i]);
			}
			printf("\n");
		*/
		decrypt(X, A_1, B, dimen, pfp);	
		}
	}
	
	if (count != 0) {
		for (i = count; i < dimen; ++i)
			X[i] = 0;
	/*	for (i = 0; i < dimen; ++i){
			printf("%d\t", X[i]);
			}            
		printf("\n");
	*/
		decrypt(X, A_1, B, dimen, pfp);
	}
	printf("\n");

	// 退出前操作
	fclose(pfp);
	fclose(cfp);

	for (i = 0; i < dimen; i++)
		free(*(A_1 + i));
	free(A_1);
	
	free(B);

	free(X);
	
	gettimeofday(&end, NULL);
	long long total_time = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	printf("Use time %lld us\n", total_time);
	
	return 0;
}

void decrypt(int* X, int** A_1, int* B, int n, FILE* fp)
{
	int i, j, C;
	
	for (i = 0; i < n; ++i)	{
		X[i] = mod(X[i] - B[i], MOD);
	}

	for (i = 0; i < n; ++i) {
		C = 0;
		for (j = 0; j < n; ++j) {
			C += mod(A_1[i][j] * X[j], MOD);
		}
		C = mod(C, MOD);
		fprintf(fp, "%c", (char)C + 65);
	}

	return;
}
