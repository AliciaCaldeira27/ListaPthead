#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1000
#define THREAD_COUNT 4

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

typedef struct {
    int start_row;
    int end_row;
} ThreadData;

void* multiply_matrices(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int i, j, k; // Declaração fora dos loops
    printf("Thread iniciada para linhas [%d, %d)\n", data->start_row, data->end_row);

    for (i = data->start_row; i < data->end_row; i++) {
        for (j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    printf("Thread finalizada para linhas [%d, %d)\n", data->start_row, data->end_row);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    ThreadData thread_data[THREAD_COUNT];
    int i, j; // Declaração fora dos loops
    int rows_per_thread; // Declaração fora do loop

    printf("Inicializando matrizes...\n");
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
    printf("Matrizes inicializadas.\n");

    rows_per_thread = SIZE / THREAD_COUNT;
    for (i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == THREAD_COUNT - 1) ? SIZE : thread_data[i].start_row + rows_per_thread;

        if (pthread_create(&threads[i], NULL, multiply_matrices, &thread_data[i]) != 0) {
            perror("Erro ao criar thread");
            return 1;
        }
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Erro ao aguardar thread");
            return 1;
        }
    }

    printf("Parte da matriz resultante C (10x10):\n");
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    printf("Multiplicação de matrizes concluída.\n");
    return 0;
}

