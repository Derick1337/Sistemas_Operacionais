#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int get_tens_and_hundreds(int pid) {
    int hundreds = (pid / 100) % 10;
    int tens     = (pid /  10) % 10;
    return hundreds * 10 + tens;
}

void collatz_sequence(unsigned int x){
    while (x != 1) {
        printf("%u ", x);
        fflush(stdout);
        if (x % 2 == 0) {
            x = x / 2;
        } else {
            x = 3 * x + 1;
        }
    }
    printf("1\n");
}

int process_quantity(void) {
    int quantity;
    while (1) {
        printf("Digite a quantidade de processos: ");
        if (scanf("%d", &quantity) == 1 && quantity > 0) {
            while (getchar() != '\n'); 
            return quantity;
        }
        printf("Quantidade inválida. Tente novamente.\n");
        while (getchar() != '\n');
    }
}

int main(){
    int i, n, status;
    pid_t x,child;
    n = process_quantity();
    for(i = 0; i < n; i++) {
        child = fork();

        if (child != 0) {
            printf("\nProcesso Pai (pid %d) executando. Filho (pid %d) criado.\n", getpid(), child);
            printf("Estado: atual = %d, pai = %d, filho = %d\n", getpid(), getppid(), child);
            printf("Pai (pid %d) esperando Filho (pid %d)\n", getpid(), child);
            x = waitpid(child, &status, 0);
            sleep(2);
            printf("Filho (pid %d) terminou\n", x);
            printf("\n[Troca de contexto]: Controle retornou ao Pai (pid %d)\n", getpid());
            printf("Estado: atual = %d, pai = %d, filho = %d (finalizado)\n", getpid(), getppid(), 0);
        } 
        else {
            printf("\n[Troca de contexto]: Processo Filho (pid %d) começou.\n", getpid());
            printf("Estado: atual = %d, pai = %d, filho = %d\n", getpid(), getppid(), 0);
            printf("Filho (pid %d) separando o número do pid em dezenas e centenas…\n", getpid());
            sleep(2);

            unsigned int num_id = get_tens_and_hundreds(getpid());
            printf("Pid %d separado em dezenas e centenas: %u\n", getpid(), num_id);
            printf("Filho (pid %d) gerando a sequência de Collatz para o número %u…\n", getpid(), num_id);
            sleep(2);
            printf("Sequência de Collatz para o número %u: ", num_id);
            collatz_sequence(num_id);
            exit(0);
        }
    }

    return 0;
}
