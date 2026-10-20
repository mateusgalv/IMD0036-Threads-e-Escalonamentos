#include <cstdlib>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdint.h>

std::vector<int> A;

void *runner(void *id) {
    int n = (int)(size_t)id;
    int mult = A[n]*A[n+1];
    printf("Sou a thread %d e o resultado da multiplicação é %d\n", n, mult);

    return nullptr;
}

int main (int argc, char* argv[]) {
    int n = std::atoi(argv[1]);
    if (n <= 0) {
        std::cerr << "Informe um valor maior que zero.\n";
        return 1;
    }
    if (n % 2 != 0) {
        std::cerr << "Numero precisa ser par.\n";
        return 1;
    }

    A.resize(n);
    for (int i = 0; i < n; i++) {
        A[i] = std::rand()%100;
    }

    pthread_t tid[n/2];

    for(int j = 0; j < n/2; j++) {
        pthread_create(&tid[j], NULL, runner, (void*)(size_t)(j));
    }

    for(int k = 0; k < n/2; k++) {
        pthread_join(tid[k], NULL);
    }

    return 0;
}