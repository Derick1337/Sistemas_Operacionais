#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(){
    int size, i, status;
    pid_t child, me, parent, x;

    while (1) {
        printf("Digite o tamanho do vetor: ");
        if (scanf("%d", &size) == 1 && size > 0) {
            break;
        } else {
            printf("Tamanho inválido. Tente novamente.\n");
            while (getchar() != '\n');
        }
    }   

    int *array1 = (int*)malloc(size * sizeof(int));
    int *array2 = (int*)malloc(size * sizeof(int));
    int *array_result = (int*)malloc(size * sizeof(int));

    if (array1 == NULL || array2 == NULL || array_result == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 1;
    }

    child = fork();
    me = getpid();
    parent = getppid();
    status = 0;

    if (child != 0) {
        printf("\nProcesso Pai (pid %d) executando. Filho (pid %d) criado.\n", getpid(), child);
        printf("Estado: atual = %d, pai = %d, filho = %d\n", getpid(), getppid(), child);

        printf("Pai (pid %d) esperando Filho (pid %d)\n", getpid(), child);
        x = waitpid(child, &status, 0);
        sleep(2);
        printf("Filho (pid %d) terminou\n", x);

        printf("\n[Troca de contexto]: Controle retornou ao Pai (pid %d)\n", getpid());
        printf("Estado: atual = %d, pai = %d, filho = %d (finalizado)\n", getpid(), getppid(), 0);

        sleep(2);
        printf("Pai (pid %d) saiu.\n", getpid());

        printf("Programa finalizado.\n");

        exit(0);
    } else {

        printf("\n[Troca de contexto]: Processo Filho (pid %d) começou.\n", getpid());
        printf("Estado: atual = %d, pai = %d, filho = %d\n", getpid(), getppid(), 0);

        srand(time(NULL) ^ getpid());
        printf("Filho (pid %d) gerando vetores aleatórios…\n", getpid());
        sleep(2);
        for (i = 0; i < size; i++) {
            array1[i] = rand() % 100;
            array2[i] = rand() % 100;
            printf("array1[%d] = %d, array2[%d] = %d\n", i, array1[i], i, array2[i]);
            sleep(2);
        }

        printf("Filho (pid %d) multiplicando vetores …\n", getpid());
        for (i = 0; i < size; i++) {
            array_result[i] = array1[i] * array2[i];
            printf("array1[%d] * array2[%d] = %d * %d = %d\n", i, i, array1[i], array2[i], array_result[i]);
            sleep(2);
        }
    }
    return 0;
}
