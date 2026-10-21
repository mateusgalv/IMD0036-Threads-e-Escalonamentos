# Threads e Escalonamento

Projeto de estudo da disciplina de Sistemas Operacionais sobre criação de threads POSIX (pthreads), execução concorrente e escalonamento em CPUs no Linux. Os exemplos usam C++ e a API pthreads.

## Conteúdo

Os programas ficam no diretório `src/` e devem ser compilados e executados separadamente:

| Programa | O que demonstra |
| --- | --- |
| `intro.cpp` | Configuração de atributos de threads, escopo de contenção (`PTHREAD_SCOPE_SYSTEM`/`PTHREAD_SCOPE_PROCESS`), afinidade de CPU e consulta da CPU em que cada thread está executando. Cria uma thread por CPU online. |
| `exerc_1.cpp` | Cria uma thread para cada posição de dois vetores e calcula a soma dos valores correspondentes. |
| `exerc_2.cpp` | Cria `n/2` threads para multiplicações entre posições adjacentes do vetor (`A[j] * A[j+1]`). As multiplicações usam índices consecutivos e podem se sobrepor. |
| `exerc_3.cpp` | Cria `n/2` threads para multiplicar pares não sobrepostos do vetor e configura a afinidade de cada thread para uma CPU, distribuindo-as em rodízio. Também informa a CPU observada durante a execução. |

## Compilação

Na raiz do projeto, compile cada exemplo separadamente:

```bash
mkdir -p build
g++ -std=gnu++17 -Wall -Wextra -pthread src/intro.cpp -o build/intro
g++ -std=gnu++17 -Wall -Wextra -pthread src/exerc_1.cpp -o build/exerc_1
g++ -std=gnu++17 -Wall -Wextra -pthread src/exerc_2.cpp -o build/exerc_2
g++ -std=gnu++17 -Wall -Wextra -pthread src/exerc_3.cpp -o build/exerc_3
```

## Execução

`intro` não recebe argumentos. Os exercícios recebem um inteiro positivo indicando o tamanho dos vetores; para `exerc_2` e `exerc_3`, esse valor também precisa ser par.

```bash
./build/intro
./build/exerc_1 8
./build/exerc_2 8
./build/exerc_3 8
```

As linhas produzidas pelas threads podem aparecer em ordens diferentes a cada execução, pois a ordem de escalonamento não é determinística. Em `intro`, a implementação Linux normalmente oferece apenas `PTHREAD_SCOPE_SYSTEM`; por isso, a tentativa de selecionar `PTHREAD_SCOPE_PROCESS` pode não ser aceita. A afinidade restringe em quais CPUs uma thread pode executar, mas não determina quando ela será escalonada.

## Observações

- Os programas usam `sched_getcpu`, `pthread_attr_setaffinity_np` e macros `CPU_*`, interfaces específicas do Linux/GNU.
- Os exercícios acessam `argv[1]` e pressupõem que ele foi fornecido.
