#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "det.h"

#define PRIME 12

const int Prime[PRIME] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25 };
void random_matrix(int** matrix, int dimension);
int find_element(int e);

int
main(int argc, char** argv)
{
	int dimen, i, d;

	if (argc != 5)
		my_err_quit("usage: generatekeyA -c <Dimension> -o <Output File for Key A>");
	
	if ((dimen = (int)strtol(argv[2],NULL,10)) <= 0)
		my_err_quit("<Dimension> must be a number bigger than 0");
	
	if (dimen == 1) {
		printf("1\n");
		return 0;
	}

	int** Matrix = (int**)malloc(dimen * sizeof(int *));
	for (i = 0; i < dimen; ++i)
		Matrix[i] = (int*)malloc(dimen * sizeof(int));
	
	Matrix[0][0] = 1;


	for (i = 1;  ;++i) {
		printf("Tring %d key...\n", i);
		random_matrix(Matrix, dimen);
/*		elementary_transformation(Matrix, dimen);
		print_matrix(Matrix, dimen);
		printf("-------------------------------------------------\n");

		sort_row_by_zero(Matrix, dimen);
		print_matrix(Matrix, dimen);
		printf("-------------------------------------------------\n");
		printf("\n");
*/

		d = det(Matrix, dimen);
		if ((find_element(d)) >= 0){
			print_matrix(Matrix, dimen);
			printf("\nDetermination: %d\n", d);
			write_matrix(argv[4], Matrix, dimen);
			break;
		}
	}

	for (i = 0; i < dimen; i++)
		free(*(Matrix+i));
	free(Matrix);

	return 0;
}

void random_matrix(int** matrix, int dimension)
{
	int temp;

	again:
	
	srand((unsigned int)time(NULL));
	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension; ++j)
		{
			temp = rand() % MOD;
			if (i == 0 && j == 0) {
				if (temp == matrix[0][0]) {
					goto again;
				}
			}
			matrix[i][j] = temp;
		}
	}
	return;
}

int find_element(int e)
{
	int middle = PRIME / 2;
    if (e > Prime[middle])
    {
        for (int i = middle + 1; i < 12; ++i)
        {
            if (e == Prime[i])
                return 0;
        }
        return -1;
    }
    else if (e < Prime[middle])
    {
        for (int i = middle - 1; i >= 0; --i)
        {
            if (e == Prime[i])
                return 0;
        }
        return -1;
    }
    else
        return 0;  
}

