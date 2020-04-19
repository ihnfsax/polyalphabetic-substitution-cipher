#include <stdio.h>
#include <stdlib.h>
#include "det.h"

void inverse_matrix(int** M, int** N, int n);
int adjugate_matrix_ele(int** M, int n, int row, int col);

const int in_element[MOD] = {0, 1, 2, 9, 4, 21, 6, 15, 8, 3, 10, 19, 12, 13, 14, 7, 16, 23, 18, 11, 20, 5, 22, 17, 24, 25};

int
main(int argc, char** argv)
{
	int dimen, i;

	if (argc != 7)
		my_err_quit("usage: inverseA -c <Dimension> -i <Key A File> -o <Output File for Key A_1>");

	if ((dimen = (int)strtol(argv[2],NULL,10)) <= 1)
		my_err_quit("<Dimension> must be a number bigger than 1");

	int** A = (int**)malloc(dimen * sizeof(int *));
	for (i = 0; i < dimen; ++i)
		A[i] = (int*)malloc(dimen * sizeof(int));
	
	read_matrix(argv[4], A, dimen);

	int** A_1 = (int**)malloc(dimen * sizeof(int *));
	for (i = 0; i < dimen; ++i)
		A_1[i] = (int*)malloc(dimen * sizeof(int));
	
	printf("A:\n");
	print_matrix(A, dimen);
	printf("Determination: %d\n", det(A, dimen));
	
	inverse_matrix(A, A_1, dimen); // 求逆矩阵
	
	printf("A_1:\n");
	print_matrix(A_1, dimen);
	printf("Determination: %d\n", det(A_1, dimen));

	write_matrix(argv[6], A_1, dimen);
	
	// 退出前操作
	for (i = 0; i < dimen; i++)
		free(*(A + i));
	free(A);
	
	for (i = 0; i < dimen; i++)
		free(*(A_1 + i));
	free(A_1);
	
	return 0;
}

void inverse_matrix(int** M, int** N, int n)
{
	int d = det(M, n);
	int detA_1 = in_element[d];
	int i, j;
	for (i = 0; i < n; ++i)
	{
		for (j = 0; j < n; ++j)
		{
			N[i][j] = mod((detA_1 * adjugate_matrix_ele(M, n, j, i)) , MOD); // 注意转置
		}
	}
	printf("\n");

	return;
}

int adjugate_matrix_ele(int** M, int n, int row, int col)
{
	int i, j, d;
	int sign = (int)((row + col) % 2 == 0 ? 1 : -1);

	int** AM = (int**)malloc((n - 1) * sizeof(int *));
	for (i = 0; i < n - 1; ++i)
		AM[i] = (int*)malloc((n - 1) * sizeof(int));
	
	for (i = 0; i < n; ++i)
	{
		if (i == row)
			continue;
		for (j = 0; j < n; ++j)
		{
			if (j == col)
				continue;
			if (i < row && j < col)
				AM[i][j] = M[i][j];
			else if (i > row && j < col)
				AM[i - 1][j] = M[i][j];
			else if (i < row && j > col)
				AM[i][j - 1] = M[i][j];
			else if (i > row && j > col)
				AM[i - 1][j - 1] = M[i][j];
		}
	}

	d = det(AM, n - 1);

	for (i = 0; i < n - 1; i++)
		free(*(AM + i));
	free(AM);
	
	return mod(sign * d, MOD);
}
