#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main() {
    FILE *fp;
    FILE *fp_out;
    char buffer[256];
    double tempo_total = 0;
    int qtd_pings = 0;
    time_t inicio = time(NULL); // Marca o tempo inicial

    // Abre (ou cria) o arquivo de saída para escrita
    fp_out = fopen("saida.txt", "w");
    if (fp_out == NULL) {
        perror("Erro ao criar o arquivo de saída");
        exit(1);
    }

    while (time(NULL) - inicio < 60) { // Loop por 60 segundos
        fp = popen("ping -n 1 -w 1000 8.8.8.8", "r");
        if (fp == NULL) {
            perror("Erro ao executar o comando");
            exit(1);
        }

        // Processa cada linha da saída do comando ping
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            // Procura apenas pela informação do tempo ("tempo=")
            char *ptr = strstr(buffer, "tempo=");
            if (ptr) {
                double tempo;
                if (sscanf(ptr, "tempo=%lfms", &tempo) == 1) {
                    tempo_total += tempo;
                    qtd_pings++;
                    // Escreve no arquivo a informação do ping por segundo
                    fprintf(fp_out, "Ping: %.2f ms\n", tempo);
                }
            }
        }
        pclose(fp);
#ifdef _WIN32
        Sleep(1000); // Sleep em milissegundos para Windows
#else
        sleep(1); // Sleep para sistemas Unix/Linux
#endif
    }

    if (qtd_pings > 0) {
        fprintf(fp_out, "\nMedia de ping em 1 minuto: %.2f ms\n", tempo_total / qtd_pings);
    } else {
        fprintf(fp_out, "\nNenhuma resposta de ping recebida.\n");
    }

    fclose(fp_out);
    return 0;
}
