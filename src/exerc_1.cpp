#include <cstdlib>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdint.h>

std::vector<int> A, B;

void *runner(void *id) {
    int n = (int)(size_t)id;
    int soma = A[n] + B[n];
    printf("Sou a thread %d e o resultado da soma é %d\n", n, soma);

    return nullptr;
}

int main (int argc, char* argv[]) {
    int n = std::atoi(argv[1]);
    if (n <= 0) {
        std::cerr << "Informe um valor maior que zero.\n";
        return 1;
    }

    A.resize(n);
    B.resize(n);

    for (int i = 0; i < n; i++) {
        A[i] = std::rand()%100;
        B[i] = std::rand()%100;
    }

    pthread_t tid[n];

    for(int j = 0; j < n; j++) {
        pthread_create(&tid[j], NULL, runner, (void*)(size_t)j);
    }

    for(int k = 0; k < n; k++) {
        pthread_join(tid[k], NULL);
    }

    return 0;
}