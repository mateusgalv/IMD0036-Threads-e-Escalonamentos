#define _GNU_SOURCE
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <sched.h>

void *runner(void *p) {
    int cpu = sched_getcpu();
    printf("Execução da Thread %d em CPU %d\n", (int)(size_t) p, cpu);

    pthread_exit(NULL);
}

int main() {
    pthread_attr_t attr;

    // Instancia attr
    pthread_attr_init(&attr);
    // Escreve escopo
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    
    int scope;
    
    // Consulta escopo atual
    pthread_attr_getscope(&attr, &scope);
    
    // scope = 1 -> PTHREAD_SCOPE_PROCESS - PCS (Process Contention Scope)
    printf("Escopo atual -> ");
    if (scope == 1) {
        printf("PTHREAD_SCOPE_PROCESS\n");
    }
    // scope = 0 -> PTHREAD_SCOPE_SYSTEM - SCS (System Contention Scope)
    if (scope == 0) {
        printf("PTHREAD_SCOPE_SYSTEM\n");
    }
    
    // O Linux suporta apenas PTHREAD_SCOPE_SYSTEM
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
    // Consulta o escopo novamente
    pthread_attr_getscope(&attr, &scope);

    printf("tentativa setscope -> PTHREAD_SCOPE_PROCESS\n");
    printf("Escopo atual -> %s\n", scope == 0 ? "PTHREAD_SCOPE_SYSTEM" : "PTHREAD_SCOPE_PROCESS");

    // ---- AFINIDADE
    
    cpu_set_t cpu_set; // conjunto das cpus
    CPU_ZERO(&cpu_set); // conjunto = vazio
    
    // Nº cores do sistema
    int NPROC = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Numero de cores: %d\n", NPROC);
    /*
    CPU_SET(0, &cpu_set); // adiciona uma CPU ao conjunto
    CPU_SET(1, &cpu_set);
    CPU_SET(2, &cpu_set);
    CPU_SET(3, &cpu_set);
    */
    /*
    bit:   7 6 5 4 3 2 1 0
          ┌─┬─┬─┬─┬─┬─┬─┬─┐
    set:  │0│0│0│0│1│1│1│1│
          └─┴─┴─┴─┴─┴─┴─┴─┘
    */
    for(int i = 0; i < NPROC; i++) {
        CPU_SET(i, &cpu_set);
    }

    // Estabelecendo afinidade:
    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpu_set);
    
    int NTHREADS = NPROC;
    pthread_t tid[NTHREADS];

    for(int i = 0; i < NTHREADS; i++) {
        pthread_create(&tid[i], &attr, runner, (void*)(size_t)i);
    }
    for(int i = 0; i < NTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_attr_destroy(&attr);

    return 0;
}

// g++ -std=c++17 -pthread -o intro intro.cpp