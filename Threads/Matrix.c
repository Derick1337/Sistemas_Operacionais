#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int id;
    int row;
    int col;
    int **A;
    int **B;
} ThreadData;

void *transpor_elemento(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int i = data->row;
    int j = data->col;
    data->B[j][i] = data->A[i][j];

    printf("Thread %d (ID: %lu) transposicionou A[%d][%d] = %d → B[%d][%d]\n",
           data->id, pthread_self(), i, j, data->A[i][j], j, i);

    return NULL;
}

int **alocar_matriz(int rows, int cols) {
    int **mat = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        mat[i] = malloc(cols * sizeof(int));
    return mat;
}

void liberar_matriz(int **mat, int rows) {
    for (int i = 0; i < rows; i++) free(mat[i]);
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
    int m, n;
    printf("Digite o número de linhas (M): ");
    scanf("%d", &m);
    printf("Digite o número de colunas (N): ");
    scanf("%d", &n);

    int total_threads = m * n;
    printf("Número de threads: %d\n", total_threads);

    int **A = alocar_matriz(m, n);
    int **B = alocar_matriz(n, m);  // transposta

    srand(time(NULL));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 100;

    printf("\nMatriz A:\n");
    imprimir_matriz(A, m, n);

    pthread_t threads[total_threads];
    ThreadData dados[total_threads];
    int t = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            dados[t] = (ThreadData){
                .id = t,
                .row = i,
                .col = j,
                .A = A,
                .B = B
            };
            pthread_create(&threads[t], NULL, transpor_elemento, &dados[t]);
            t++;
        }
    }

    for (int i = 0; i < total_threads; i++)
        pthread_join(threads[i], NULL);

    printf("\nMatriz B (Transposta):\n");
    imprimir_matriz(B, n, m);

    liberar_matriz(A, m);
    liberar_matriz(B, n);
    return 0;
}
