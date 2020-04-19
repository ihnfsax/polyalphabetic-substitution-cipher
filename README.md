# polyalphabetic-substitution-cipher
Polyalphabetic Substitution Cipher：基于仿射变换的多表代换密码的C语言实现

# 多表代换编程实现
## 原理
![avatar](/多表代换原理.png)
## 编程语言
C
## 编程环境
Ubuntu 18.04
## 编译器
gcc
## 源代码文件说明
polyalphabetic_substitution.tar : 源代码、程序、资源文件的集合，解压后得到以下源代码文件
- det.c : 提供求解行列式的功能以及一些附加实用功能
- det.h : 头文件
- generatekeyA.c : 生成指定阶数且满足要求的矩阵A
- generatekeyB.c : 生成指定长度的数组B
- gettext.c : 提供随机生成指定长度的纯字母文本的实用功能
- encrypt.c : 加密文本
- inverseA.c : 生成矩阵A的逆矩阵A_1
- decrypt.c : 解密文本
## 准备工作
使用以下指令编译源代码 :  
```
gcc generatekeyA.c det.c -o generatekeyA
gcc generatekeyBc det.c -o generatekeyB
gcc gettext.c det.c -o gettext
gcc encrypt.c det.c -o encrypt
gcc inverseA.c det.c -o incerseA
gcc decrypt.c det.c -o decrypt
```  
注意 : polyalphabetic_substitution.tar 提供了生成好的程序文件
## 加解密过程
为了方便说明，以32阶矩阵为例 :  
1. 生成满足要求的矩阵A，指定阶数为32，写入到文件32keyA.txt，行列式可在标准输出查看  
`./generatekeyA -c 32 -o 32keyA.txt`  
2. 生成数组B，指定长度为32，写入到文件32keyB.txt  
`./generatekeyB -c 32 -o 32keyB.txt`  
3. 随机生成纯字母明文，指定长度为1000000，写入到文件plain.txt  
`./gettext -a 1000000 -o plain.txt` 
4. 加密明文，指定阶数为32，读取文件plain.txt, 32keyA.txt, 32keyB.txt，将密文写入文件cipher.txt  
`./encrypt -c 32 -i plain.txt 32keyA.txt 32keyB.txt -o cipher.txt`  
5. 生成逆矩阵A_1，指定阶数为32，读取文件32keyA.txt，写入文件32keyA_1.txt，行列式可在标准输出查看  
`./inverseA -c 16 -i 32keyA.txt -o 32keyA_1.txt`  
6. 解密密文，指定阶数为32，读取文件cipher.txt, 32keyA_1.txt, 32keyB.txt，将结果写入文件output.txt  
`./decrypt -c 32 -i cipher.txt 32keyA_1.txt 32keyB.txt -o output.txt`  

注意 : 由于未做灵活的参数解析，以上指令的参数顺序不能改动。polyalphabetic_substitution.tar 包含了所有提到的文件。
## 结果分析
1000000个字母加密 :   
- 16阶加密 : Use time 88591 us  
- 16阶解密 : Use time 96396 us  
- 32阶加密 : Use time 169211 us  
- 32阶解密 : Use time 181925 us  
## 细节
加密时，明文结尾无法凑够一组数组时，填充(padding)使用的是字母A。  
求逆矩阵用的是且只能用伴随矩阵法。  
## 关键代码
关键是求行列式的算法的优化。求行列式用的是递归求代数余子式的方法，但是要通过类似高斯消元的方法减小时间复杂度。  
```
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
```
