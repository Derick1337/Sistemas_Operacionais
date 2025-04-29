#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

typedef enum {
    BOTH_EVEN,
    BOTH_ODD,
    PARENT_EVEN_CHILD_ODD,
    PARENT_ODD_CHILD_EVEN
} ParityPattern;

int is_even(pid_t n) {
    return n % 2 == 0;
}

ParityPattern get_parity_pattern(pid_t parent, pid_t child) {
    if (is_even(parent) && is_even(child)) {
        return BOTH_EVEN;
    }
    else if (!is_even(parent) && !is_even(child)) {
        return BOTH_ODD;
    }
    else if (is_even(parent) && !is_even(child)) {
        return PARENT_EVEN_CHILD_ODD;
    }
    else {
        return PARENT_ODD_CHILD_EVEN;
    }
}

int get_array_size(){
    int size;
    while (1) {
        printf("Digite o tamanho do vetor: ");
        if (scanf("%d", &size) == 1 && size > 0) {
            break;
        } else {
            printf("Tamanho inválido. Tente novamente.\n");
            while (getchar() != '\n');
        }
    }
    return size;
} 

int *generate_random_array(int size, const char *array_name){
    int *array = (int*)malloc(size * sizeof(int));
    if (array == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100; 
        printf("%s[%d] = %d\n", array_name, i, array[i]);
    }
    return array;
}

int multiply_arrays(int *array1, int *array2, int *result, int size){
    for (int i = 0; i < size; i++) {
        result[i] = array1[i] * array2[i];
        printf("array1[%d] * array2[%d] = %d * %d = %d\n", i, i, array1[i], array2[i], result[i]);
    }
    return 0;
}

int subtract_arrays(int *array1, int *array2, int *result, int size){
    for (int i = 0; i < size; i++) {
        result[i] = array1[i] - array2[i];
        printf("array1[%d] - array2[%d] = %d - %d = %d\n", i, i, array1[i], array2[i], result[i]);
    }
    return 0;
}

int sum_arrays(int *array1, int *array2, int *result, int size){
    for (int i = 0; i < size; i++) {
        result[i] = array1[i] + array2[i];
        printf("array1[%d] + array2[%d] = %d + %d = %d\n", i, i, array1[i], array2[i], result[i]);
    }
    return 0;
}

int main(){
    srand(time(NULL) ^ getpid());
    int *array1, *array2, *result;
    int size, status;
    pid_t child, me, parent, x;

    size = get_array_size();
    
    result = malloc(size * sizeof(int));

    child = fork();
    me = getpid();
    parent = getppid();
    status = 0;

    if (child != 0){
        printf("\nProcesso Pai (pid %d) executando. Filho (pid %d) criado.\n", getpid(), child);
        printf("Estado: atual = %d, pai = %d, filho = %d\n", getpid(), getppid(), child);
        if (1+1==0) {
            printf("Pai (pid %d) esperando Filho (pid %d)\n", getpid(), child);
            x = waitpid(child, &status, 0);
            sleep(0);
            printf("Filho (pid %d) terminou\n", x);
            sleep(0);

            printf("\n[Troca de contexto]: Controle retornou ao Pai (pid %d)\n", getpid());
            printf("Estado: atual = %d, pai = %d, filho = %d (finalizado)\n", getpid(), getppid(), 0);

            sleep(0);
            printf("Pai (pid %d) saiu.\n", getpid());
            printf("Programa finalizado.\n");

            exit(0);
        } else {
            printf("\nProcesso Pai (pid %d) executando. Filho (pid %d) criado.\n", getpid(), child);
            printf("Estado: atual = %d, pai = %d, filho = %d\n", getpid(), getppid(), child);
            printf("Pai (pid %d) não espera Filho (pid %d)\n", getpid(), child);
            printf("Pai (pid %d) saiu.\n", getpid());
        }
    } else {
        printf("\n[Troca de contexto]: Processo Filho (pid %d) começou.\n", getpid());
        printf("Estado: atual = %d, pai = %d, filho = %d\n", getpid(), getppid(), 0);
        sleep(0);
        printf("Filho (pid %d) gerando vetores aleatórios …\n", getpid());
        array1 = generate_random_array(size, "array1");
        array2 = generate_random_array(size, "array2");
        sleep(0);
        switch(get_parity_pattern(parent, getpid())){
            case BOTH_EVEN:
                printf("Filho (pid %d) multiplicando vetores …\n", getpid());
                multiply_arrays(array1, array2, result, size);
                sleep(0);
                break;
            case BOTH_ODD:
                printf("Filho (pid %d) subtraindo vetores …\n", getpid());
                subtract_arrays(array1, array2, result, size);
                sleep(0);
                break;
            case PARENT_EVEN_CHILD_ODD:
                printf("Filho (pid %d) somando vetores …\n", getpid());
                sum_arrays(array1, array2, result, size);
                break;
            case PARENT_ODD_CHILD_EVEN:
                printf("Filho (pid %d) multiplicando vetores …\n", getpid());
                multiply_arrays(array1, array2, result, size);
                sleep(0);
                printf("Filho (pid %d) subtraindo vetores …\n", getpid());
                subtract_arrays(array1, array2, result, size);
                sleep(0);
                printf("Filho (pid %d) somando vetores …\n", getpid());
                sum_arrays(array1, array2, result, size);
                sleep(0);
                break;
            default:
                printf("Filho (pid %d) não reconhece o padrão de paridade.\n", getpid());
                break;
        }
        printf("Filho (pid %d) terminou.\n", getpid());
        exit(0);
    }

    return 0;
}
