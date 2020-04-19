#define MOD 26

int det(int** M, int n);

int mod(int n, int mod);

void copy_matrix(int** M1, int** M2, int dimension);

void elementary_transformation(int** M, int n);

int sort_row_by_zero(int **M, int n);

// 以下与求行列式无关
void print_matrix(int** matrix, int dimension);

void my_err_quit(const char *fmt);

void read_matrix(const char* filename, int** M, int n);

void read_array(const char* filename, int* M, int n);

void write_matrix(const char* filename, int** M, int n);

void write_array(const char* filename, const int* M, int n);

