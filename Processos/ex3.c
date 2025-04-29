#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

void Collatz(unsigned int n){
    printf("Sequência de Collatz para %u :\n", n);
    while (n != 1) {
        printf("%u ", n);
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
    }
    printf("%u\n", n);
}

int main(){
    unsigned int numero;

    printf("Digite um número inteiro positivo: ");
    scanf("%u", &numero);

    if (numero <= 0) {
        printf("Número inválido. Por favor, insira um número inteiro positivo.\n");
        return 1;
    } else {
        Collatz(numero);
    }
    pid_t newPid, me, parent, x;
    int status;

    newPid = fork();

    me = getpid();
    parent = getppid();

    printf("%d, %d, %d\n", me, parent, newPid);

    if (newPid != 0)
    {
        printf("Esperando filho pid %d\n", newPid);
        x = waitpid(newPid, &status, 0);
        printf("Filho %d terminou\n", x);
    }
    else
    {
        printf("Sou o filho\n");
        sleep(5);
    }

    return 0;
}