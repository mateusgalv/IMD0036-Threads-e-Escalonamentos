#include <cstdlib>
#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#include <ctime>

std::vector<int> A;

void *runner(void *id) {
    int n = (int)(size_t)id;
    int mult = A[2*n]*A[2*n+1];
    int cpu = sched_getcpu();
    printf("Sou a thread %d na cpu %d e o resultado da multiplicação é %d\n", n, cpu, mult);

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

    std::srand((unsigned)std::time(nullptr));
    A.resize(n);
    for (int i = 0; i < n; i++) {
        A[i] = std::rand() % 100;
    }

    int NPROC = sysconf(_SC_NPROCESSORS_ONLN);
    int NTHREADS = n/2;
    
    printf("Numero de CPUs disponíveis: %d\n", NPROC);
    
    pthread_t tid[NTHREADS];
    
    for(int j = 0; j < NTHREADS; j++) {
        int cpu = j % NPROC;

        pthread_attr_t attr;
        pthread_attr_init(&attr);

        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(cpu, &cpu_set);

        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpu_set);

        pthread_create(&tid[j], &attr, runner, (void*)(size_t)j);

        pthread_attr_destroy(&attr);
    }

    for(int k = 0; k < n/2; k++) {
        pthread_join(tid[k], NULL);
    }

    return 0;
}