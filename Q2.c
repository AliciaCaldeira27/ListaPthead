#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define MAX 100000
#define THREAD_COUNT (MAX / 1000)

typedef struct {
    int start;
    int end;
} Range;

// O Mutex para sincronizar a impress�o
pthread_mutex_t print_mutex;

// A fun��o para verificar se um n�mero � primo
int is_prime(int num) {
    if (num < 2) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;
    int i;
    for (i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) return 0;
    }
    return 1;
}
// A fun��o executada pelas threads
void* find_primes(void* arg) {
    Range* range = (Range*)arg;
    int i;
    printf("Thread iniciada para o intervalo [%d, %d)\n", range->start, range->end);

    for (i = range->start; i < range->end; i++) {
        if (is_prime(i)) {
            pthread_mutex_lock(&print_mutex);
            printf("Primo encontrado: %d\n", i);
            pthread_mutex_unlock(&print_mutex);
        }
    }

    printf("Thread finalizada para o intervalo [%d, %d)\n", range->start, range->end);
    free(range); // Libera memoria do intervalo
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int thread_id;
    pthread_mutex_init(&print_mutex, NULL);

    for (thread_id = 0; thread_id < THREAD_COUNT; thread_id++) {
    	// Determina o intervalo de cada thread
        Range* range = (Range*)malloc(sizeof(Range));
        if (range == NULL) {
            perror("Erro ao alocar mem�ria para o intervalo");
            return 1;
        }
        range->start = thread_id * 1000;
        range->end = range->start + 1000;
        
		// Cria a thread
        if (pthread_create(&threads[thread_id], NULL, find_primes, (void*)range) != 0) {
            perror("Erro ao criar a thread");
            return 1;
        }
    }
    
	// Aguardar todas as threads terminarem
    for (thread_id = 0; thread_id < THREAD_COUNT; thread_id++) {
        if (pthread_join(threads[thread_id], NULL) != 0) {
            perror("Erro ao aguardar a thread");
            return 1;
        }
    }
    
	// Destruir o mutex
    pthread_mutex_destroy(&print_mutex);
    printf("C�lculo de n�meros primos conclu�do.\n");
    return 0;
}
