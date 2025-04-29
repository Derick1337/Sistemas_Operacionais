#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *tasks;
    int capacity;
    int count;
    int idx;
    int shutdown;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
} task_queue;

typedef struct {
    int rows, cols;
    int **A, **B, **C;
    task_queue *queue;
} thread_data;

int **allocate_matrix(int rows, int cols) {
    int **m = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        m[i] = malloc(cols * sizeof(int));
    return m;
}

void fill_random_matrix(int rows, int cols, int **m) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            m[i][j] = rand() % 100;
}

void print_matrix(int rows, int cols, int **m) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%4d ", m[i][j]);
        printf("\n");
    }
}

void get_matrix_size(int *rows, int *cols) {
    do {
        printf("Digite o número de linhas: ");
        if (scanf("%d", rows) != 1 || *rows <= 0) {
            printf("Entrada inválida! Inteiro positivo.\n");
            while (getchar() != '\n');
            *rows = 0;
        }
    } while (*rows <= 0);
    do {
        printf("Digite o número de colunas: ");
        if (scanf("%d", cols) != 1 || *cols <= 0) {
            printf("Entrada inválida! Inteiro positivo.\n");
            while (getchar() != '\n');
            *cols = 0;
        }
    } while (*cols <= 0);
}

int get_threads_count(int max) {
    int t;
    do {
        printf("Digite o número de threads (até %d): ", max);
        if (scanf("%d", &t) != 1 || t <= 0 || t > max) {
            printf("Inválido! Entre 1 e %d.\n", max);
            while (getchar() != '\n');
            t = 0;
        }
    } while (t <= 0);
    return t;
}

void init_queue(task_queue *q, int capacity) {
    q->tasks = malloc(sizeof(int) * capacity);
    q->capacity = capacity;
    q->count = 0;
    q->idx = 0;
    q->shutdown = 0;
    pthread_mutex_init(&q->mtx, NULL);
    pthread_cond_init(&q->cond, NULL);
}

void add_task(task_queue *q, int task) {
    pthread_mutex_lock(&q->mtx);
    if (q->count < q->capacity) {
        q->tasks[q->count++] = task;
        pthread_cond_signal(&q->cond);
    }
    pthread_mutex_unlock(&q->mtx);
}

int get_task(task_queue *q) {
    pthread_mutex_lock(&q->mtx);
    while (q->idx == q->count && !q->shutdown) {
        pthread_cond_wait(&q->cond, &q->mtx);
    }
    int task = -1;
    if (q->idx < q->count)
        task = q->tasks[q->idx++];
    pthread_mutex_unlock(&q->mtx);
    return task;
}

void *sum_matrices(void *arg) {
    thread_data *d = arg;
    int total = d->rows * d->cols;
    while (1) {
        int idx = get_task(d->queue);
        if (idx == -1) break;
        int i = idx / d->cols;
        int j = idx % d->cols;
        d->C[i][j] = d->A[i][j] + d->B[i][j];
        printf("Thread %lu fez idx %2d → C[%d][%d] = %d + %d = %d\n",
               pthread_self(), idx, i, j,
               d->A[i][j], d->B[i][j], d->C[i][j]);
    }
    return NULL;
}

int main() {
    int rows, cols;
    get_matrix_size(&rows, &cols);

    int **A = allocate_matrix(rows, cols);
    int **B = allocate_matrix(rows, cols);
    int **C = allocate_matrix(rows, cols);
    fill_random_matrix(rows, cols, A);
    fill_random_matrix(rows, cols, B);

    printf("\nMatriz A:\n"); print_matrix(rows, cols, A);
    printf("\nMatriz B:\n"); print_matrix(rows, cols, B);

    int total = rows * cols;
    int T = get_threads_count(total);

    task_queue queue;
    init_queue(&queue, total);
    pthread_t threads[T];
    thread_data td[T];

    for (int t = 0; t < T; t++) {
        td[t].rows = rows;
        td[t].cols = cols;
        td[t].A = A;
        td[t].B = B;
        td[t].C = C;
        td[t].queue = &queue;
        pthread_create(&threads[t], NULL, sum_matrices, &td[t]);
    }

    for (int i = 0; i < total; i++)
        add_task(&queue, i);

    pthread_mutex_lock(&queue.mtx);
    queue.shutdown = 1;
    pthread_cond_broadcast(&queue.cond);
    pthread_mutex_unlock(&queue.mtx);

    for (int t = 0; t < T; t++)
        pthread_join(threads[t], NULL);

    printf("\nMatriz C (Resultado):\n");
    print_matrix(rows, cols, C);

    for (int i = 0; i < rows; i++) {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);
    free(queue.tasks);
    return 0;
}