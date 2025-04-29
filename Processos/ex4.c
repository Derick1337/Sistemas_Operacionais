#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Função para pedir o caminho do programa a ser executado
void order_path(char *path) {
    printf("Digite o caminho do programa que o processo filho deve executar:\n");
    scanf("%255s", path);  // Lê o caminho do executável
}

int main() {
    char path[256];  // Variável para armazenar o caminho do programa

    // 1. Pede o caminho do executável ao usuário
    order_path(path);

    // 2. Cria o processo filho
    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro ao criar o processo filho");
        return 1;
    }

    if (pid == 0) {
        // 3. Processo filho executa o programa
        printf("Filho (PID %d): executando '%s'\n", getpid(), path);
        
        // Sleep para simular um pequeno atraso antes de executar o programa
        sleep(2);  // Espera 2 segundos antes de executar o execl

        // Executa o programa com execl
        execl(path, path, (char *)NULL);

        // Só chega aqui se execl falhar
        perror("execl falhou");
        exit(1);
    } else {
        // 4. Processo pai espera o filho
        int status;
        printf("Pai (PID %d): esperando o filho (PID %d) terminar...\n", getpid(), pid);

        // Sleep para simular o tempo que o pai espera antes de detectar a finalização
        sleep(2);  // O pai dorme 2 segundos para simular a espera

        waitpid(pid, &status, 0);  // Espera o filho terminar
        printf("Pai (PID %d): processo filho terminou\n", getpid());
    }

    return 0;
}
