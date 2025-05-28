#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int id;
    int linhas, colunas;
    int total_threads;
    int **A, **B;
} ThreadData;

void *transpor_varios_elementos(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int id = data->id;
    int linhas = data->linhas;
    int colunas = data->colunas;
    int total = linhas * colunas;

    // Cada thread pega blocos de elementos
    for (int idx = id; idx < total; idx += data->total_threads) {
        int i = idx / colunas;
        int j = idx % colunas;
        data->B[j][i] = data->A[i][j];
        printf("Thread %d (ID: %lu) transposicionou A[%d][%d] = %d → B[%d][%d]\n",
               id, pthread_self(), i, j, data->A[i][j], j, i);
    }

    return NULL;
}

int **alocar_matriz(int rows, int cols) {
    int **mat = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        mat[i] = malloc(cols * sizeof(int));
    return mat;
}

void liberar_matriz(int **mat, int rows) {
    for (int i = 0; i < rows; i++)
        free(mat[i]);
    free(mat);
}

void imprimir_matriz(int **mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%3d ", mat[i][j]);
        printf("\n");
    }
}

int main() {
    int linhas, colunas, num_threads;

    printf("Digite o número de linhas (M): ");
    scanf("%d", &linhas);
    printf("Digite o número de colunas (N): ");
    scanf("%d", &colunas);

    int total_elementos = linhas * colunas;
    printf("Número máximo de threads permitidas: %d\n", total_elementos);
    printf("Digite o número de threads (1 a %d): ", total_elementos);
    scanf("%d", &num_threads);

    if (num_threads < 1 || num_threads > total_elementos) {
        printf("Número de threads inválido!\n");
        return 1;
    }

    int **A = alocar_matriz(linhas, colunas);
    int **B = alocar_matriz(colunas, linhas);

    srand(time(NULL));
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            A[i][j] = rand() % 100;

    printf("\nMatriz A:\n");
    imprimir_matriz(A, linhas, colunas);

    pthread_t threads[num_threads];
    ThreadData dados[num_threads];

    for (int t = 0; t < num_threads; t++) {
        dados[t] = (ThreadData){
            .id = t,
            .linhas = linhas,
            .colunas = colunas,
            .total_threads = num_threads,
            .A = A,
            .B = B
        };
        pthread_create(&threads[t], NULL, transpor_varios_elementos, &dados[t]);
    }

    for (int t = 0; t < num_threads; t++)
        pthread_join(threads[t], NULL);

    printf("\nMatriz B (Transposta):\n");
    imprimir_matriz(B, colunas, linhas);

    liberar_matriz(A, linhas);
    liberar_matriz(B, colunas);
    return 0;
}
