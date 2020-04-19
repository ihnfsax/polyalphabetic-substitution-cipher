#include <stdio.h>
#include <stdlib.h>
#include "det.h"

int sort_index(int* array, int n, int* index);
int det_re(int** M, int n);

int det(int** M, int n)
{
	int i, d;

	int** M2 = (int**)malloc(n * sizeof(int *));
	for (i = 0; i < n; ++i)
		M2[i] = (int*)malloc(n * sizeof(int));
	
	copy_matrix(M, M2, n);

	// 把M2拿去算行列式，保护M1不被修改
	d = det_re(M2, n);

	for (i = 0; i < n; i++)
		free(*(M2+i));
	free(M2);

	return d;
}


// 递归计算代数余子式求行列式
int det_re(int** M, int n)
{	    
	if (n == 3)
		return mod((M[0][0] * M[1][1] * M[2][2]) + (M[0][1] * M[1][2] * M[2][0]) + (M[0][2] * M[1][0] * M[2][1]) - \
		((M[0][0] * M[1][2] * M[2][1]) + (M[0][1] * M[1][0] * M[2][2]) + (M[0][2] * M[1][1] * M[2][0])), MOD);
	// 降低时间复杂的两个操作
	elementary_transformation(M, n);
//	if (n >= 16)
	int count = sort_row_by_zero(M, n);
	int sign = (int)(count % 2 == 0 ? 1 : -1);

	// 正式计算
	int i, j, k;
    int len = n - 1; /*子行列式的阶*/
    int s = 0;
    int** p = (int**)malloc(len * sizeof(int*));
    for (int i = 0; i < len; i++)
        p[i] = (int*)malloc(len * sizeof(int));
    for (k = 0; k < n; k++)
    {
		if (M[0][k] == 0)	// 如果为0,就没必要继续往下算了，这就是减少时间复杂度的关键
			continue;
        for (i = 0; i < len; i++) {
            for (j = 0; j < len; j++)
            {
                if (j < k) {
                    p[i][j] = mod(M[i + 1][j], MOD);
                }
                else {
                    p[i][j] = mod(M[i + 1][j + 1], MOD);
                }
            }
        }
        s += (int)(k % 2 == 0 ? 1 : -1) * M[0][k] * det(p, len); /*递归计算*/
    }

    for (i = 0; i < len; i++)
        free(*(p + i));
    free(p);

    return mod(sign * s, MOD);
}

// 初等变换，减小时间复杂度的核心之一，输入的矩阵M需要提前分配空间，n为矩阵大小
void elementary_transformation(int **M, int n)
{
	int i, j, k, temp, r, p, flag = 0;
	for (p = n - 1; p >= 0; --p){ // 行，从下往上
		for (r = 1; r < 4; ++r) { // 初等变换中小的那个数，从1到3都试一下
			for (i = 0; i < n; ++i) // 列
			{
				if (M[p][i] == r) {
					for (j = 0; j < n; ++j) { // 列
						if (M[p][j] == 0 || j == i)
							continue;
						temp = M[p][j];
						if (temp % r == 0) {
							for (k = 0; k < n; ++k) { // 行
								if (M[k][i] != 0)	
									M[k][j] = mod(M[k][j] - (int)(temp / r) * M[k][i], MOD);
							}
						}
					}
					if (r == 1) { // 如果有1可以变换，2和3都可以不用试了
						flag = 1;
					}
					break;
				}
			}
			if (flag == 1) {
				flag = 0;
				break;
			}
		}
	}

	return;
}

// 根据0的个数对矩阵M（提前了分配大小）的行排序，0多的行排在上面，n为矩阵大小。这也是一种初等变换，返回的是交换行的次数
int  sort_row_by_zero(int **M, int n)
{
	int *zero_array = (int*)calloc(n, sizeof(int));
	int *zero_index = (int*)calloc(n, sizeof(int));
	int i, j, temp, count = 0;
	
	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			if (M[i][j] == 0)
				zero_array[i] = zero_array[i] + 1;
		}
	}

	count = sort_index(zero_array, n, zero_index);

	free(zero_array);

	int** M2 = (int**)malloc(n * sizeof(int *));
    for (i = 0; i < n; ++i)
		M2[i] = (int*)malloc(n * sizeof(int));

	for (i = 0; i < n; ++i){
		temp = zero_index[i];
		for (j = 0; j < n; ++j){
			M2[i][j] = M[temp][j];
		}
	}

	copy_matrix(M2, M, n);

	free(zero_index);

	for (i = 0; i < n; i++)
		free(*(M2+i));
	free(M2);

	return count;
}

// 对数组进行排序，并存储新数组在原数组的索引，输入数组array，数组大小n，输出排序后数组array，索引数组index。（都需要提前分配大小）
int sort_index(int* array, int n, int* index)
{
    int i, j, temp1, temp2, count = 0;
	
	for (i = 0; i < n; ++i)
		index[i] = i;

    for (i = 0; i < n - 1; i++) {
        for(j = 0; j < n - 1 -i; j++)
            if(array[j] < array[j+1])
            {
				temp1 = array[j];
				array[j] = array[j+1];
				array[j+1] = temp1;

                temp2 = index[j];
                index[j] = index[j+1];
                index[j+1] = temp2;

				count++;
            }
	}

	return count;
}

// 取余函数，输入数字n，模数mod，返回取余结果
int mod(int n, int mod)
{
    if (mod <= 0)
        return n;
    for (; n < 0;)
    {
        n += mod;
    }
    return (n % mod);
}

// 复制矩阵内容，将M1的内容复制到M2（都要提前分配空间），dimension是矩阵大小。一般作备份数据的作用。
void copy_matrix(int** M1, int** M2, int dimension)
{
     int i, j;
     for (i = 0; i < dimension; ++i)
     {
         for (j = 0; j < dimension; ++j)
		 {
             M2[i][j] = M1[i][j];
         }
     }
     return;
}

// 打印矩阵，与求行列式无关
void print_matrix(int** matrix, int dimension)
{
	for (int i = 0; i < dimension; ++i)
	{
		for (int j = 0; j < dimension - 1; ++j)
		{
			printf("%d\t", matrix[i][j]);
		//	printf("%d,", matrix[i][j]);
		}
		printf("%d\n", matrix[i][dimension - 1]);
//		printf("%d;\n", matrix[i][dimension - 1]);
	}
	return;
}

// 输出错误信息，并退出，与求行列式无关
void my_err_quit(const char *fmt)
{
	fflush(stdout);
	fputs(fmt, stderr);
	fputc('\n', stderr);
	fflush(stderr);
	exit(1);
}

// 从文件filename装载矩阵，与求行列式无关
void read_matrix(const char* filename, int** M, int n)
{
	int i, j;
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL)
		perror("fopen error"),exit(1);
		
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n; ++j){
			fscanf(fp, "%d", &M[i][j]);
		}
	}

	fclose(fp);
	return;
}

// 从文件filename装载数组，与求行列式无关
void read_array(const char* filename, int* M, int n)
{
	int i;
	FILE *fp = NULL;
	if((fp = fopen(filename,"r")) == NULL)
		perror("fopen error"),exit(1);

	for(i = 0; i < n; ++i) {
		fscanf(fp, "%d", &M[i]);
	}

	fclose(fp);
	return;
}

// 写矩阵到文件，与求行列式无关
void write_matrix(const char* filename, int** M, int n)
{
	int i, j;
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL)
        perror("fopen error"),exit(1);
	
	for(i = 0; i < n; ++i) {
		for(j = 0; j < n - 1; ++j){
			fprintf(fp, "%d\t", M[i][j]);
		}
		fprintf(fp, "%d\n", M[i][n - 1]);
	}

	fclose(fp);
	return;
}

// 写数组到文件，与求行列式无关
void write_array(const char* filename, const int* M, int n)
{
	int i;
	FILE *fp = NULL;
	if((fp = fopen(filename,"w")) == NULL)
		perror("fopen error"),exit(1);
	
	for(i = 0; i < n - 1; ++i) {
		fprintf(fp, "%d\t", M[i]);
	}
	fprintf(fp, "%d", M[n - 1]);

	fclose(fp);
	return;
}

