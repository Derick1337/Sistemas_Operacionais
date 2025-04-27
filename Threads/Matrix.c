#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main()
{
    int m, n, num_threads;
    int A [m][n];
    int B [n][m];
    printf("Digite o número de linhas (M):");
    scanf("%d", &m);
    printf("Digite o número de colunas (N):");
    scanf("%d", &n);
    printf("Digite o número de threads:");
    scanf("%d", &num_threads);

    srand(time(NULL));
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            A[i][j] = rand() % 100;
        }
    }

    int linhas_por_thread = m / num_threads;


    printf("\nMatriz A:\n");
    for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
        printf("%d ", A[i][j]);
    }
    printf("\n");
}

    printf("\nMatriz B (Transposta):\n");
    for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
        printf("%d ", B[i][j]);
    }
    printf("\n");
}

   

   return 0;
}