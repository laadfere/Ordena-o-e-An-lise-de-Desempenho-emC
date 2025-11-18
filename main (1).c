#include <stdio.h> // Inclui a biblioteca padrão de entrada e saída (para printf, scanf, etc.)
#include <stdlib.h> // Inclui a biblioteca padrão (para malloc, free, exit, rand, srand)
#include <string.h> // Inclui a biblioteca de manipulação de strings (para strlen)
#include <time.h> // Inclui a biblioteca de tempo (para clock, time, CLOCKS_PER_SEC)

// --- MÉTRICAS ---
typedef struct { // Define uma estrutura para armazenar métricas de desempenho
    long long cmp; // Contador de comparações
    long long swap; // Contador de trocas (movimentações de elementos)
} Metrics;

void reset_metrics(Metrics *m) { // Função para zerar os contadores de uma estrutura Metrics
    m->cmp = 0; // Zera o contador de comparações
    m->swap = 0; // Zera o contador de trocas
}

void esperar_enter() { // Função para pausar a execução até que o usuário pressione ENTER
    int c; // Variável para armazenar caracteres lidos
    printf("\nPressione ENTER para continuar..."); // Pede ao usuário para pressionar ENTER
    // Limpa o buffer de entrada, consumindo caracteres restantes da entrada anterior (incluindo o '\n' que pode ter sobrado)
    while ((c = getchar()) != '\n' && c != EOF) {}
    getchar(); // Espera a leitura do próximo caractere, que deve ser o ENTER
}

#define COUNT_CMP(m)  ((m)->cmp++) // Macro para incrementar o contador de comparações de forma concisa
#define COUNT_SWAP(m) ((m)->swap++) // Macro para incrementar o contador de trocas de forma concisa

// --- SELECTION SORT ---
// Implementação do algoritmo Selection Sort
void selection_sort(int *v, int n, Metrics *m) {
    for (int i = 0; i < n - 1; i++) { // Loop externo: percorre o array, do primeiro ao penúltimo elemento
        int min = i; // Assume que o elemento atual (v[i]) é o menor na sub-lista não ordenada
        for (int j = i + 1; j < n; j++) { // Loop interno: busca o menor elemento no restante do array
            COUNT_CMP(m); // Conta a comparação entre v[j] e v[min]
            if (v[j] < v[min]) min = j; // Se v[j] for menor que o mínimo atual, atualiza o índice do mínimo
        }
        if (min != i) { // Se o menor elemento não for o que já estava na posição correta (v[i])
            int t = v[i]; v[i] = v[min]; v[min] = t; // Realiza a troca do elemento atual com o menor encontrado
            COUNT_SWAP(m); // Conta a troca realizada
        }
    }
}

// ---INSERTION SORT ---
// Implementação do algoritmo Insertion Sort
void insertion_sort(int *v, int n, Metrics *m) {
    for (int i = 1; i < n; i++) { // Loop externo: percorre o array, começando do segundo elemento (índice 1)
        int key = v[i]; // Armazena o valor do elemento a ser inserido na sub-lista ordenada
        int j = i - 1; // Índice do último elemento da sub-lista ordenada
        while (j >= 0) { // Loop interno: move elementos da sub-lista ordenada (da direita para a esquerda)
            COUNT_CMP(m); // Conta a comparação entre v[j] e a "key"
            if (v[j] <= key) break; // Se v[j] for menor ou igual à "key", a posição de inserção foi encontrada
            v[j + 1] = v[j]; // Move v[j] uma posição para a direita
            COUNT_SWAP(m); // Conta a movimentação (troca de posição, embora apenas uma atribuição)
            j--; // Vai para o próximo elemento à esquerda
        }
        v[j + 1] = key; // Insere a "key" na posição correta
    }
}

// --- QUICK SORT ---
// Função auxiliar para a partição (pivotamento) no Quick Sort
int partition(int *v, int low, int high, Metrics *m) {
    int pivot = v[high]; // Escolhe o último elemento como pivô
    int i = low - 1; // Índice do menor elemento (próximo lugar para uma troca)
    for (int j = low; j < high; j++) { // Percorre a sub-lista (excluindo o pivô)
        COUNT_CMP(m); // Conta a comparação de v[j] com o pivô
        if (v[j] < pivot) { // Se o elemento atual for menor que o pivô
            i++; // Incrementa o índice do menor elemento
            int t = v[i]; v[i] = v[j]; v[j] = t; // Troca v[i] com v[j]
            COUNT_SWAP(m); // Conta a troca
        }
    }
    int t = v[i + 1]; v[i + 1] = v[high]; v[high] = t; // Coloca o pivô na posição correta (depois do último elemento menor)
    COUNT_SWAP(m); // Conta a troca do pivô
    return i + 1; // Retorna o índice do pivô
}

// Função recursiva principal do Quick Sort
void quick_sort_rec(int *v, int low, int high, Metrics *m) {
    if (low < high) { // Condição de parada: a sub-lista deve ter pelo menos dois elementos
        int pi = partition(v, low, high, m); // Particiona o array e obtém o índice do pivô
        quick_sort_rec(v, low, pi - 1, m); // Ordena recursivamente a sub-lista à esquerda do pivô
        quick_sort_rec(v, pi + 1, high, m); // Ordena recursivamente a sub-lista à direita do pivô
    }
}

// Função de fachada para o Quick Sort (inicia a recursão)
void quick_sort(int *v, int n, Metrics *m) {
    quick_sort_rec(v, 0, n - 1, m); // Chama a função recursiva para todo o array
}

// --- RODAR SORT + TEMPO ---
// Função genérica para executar um algoritmo de ordenação, medir tempo e métricas
double run_sort(void (*fn)(int*,int,Metrics*), int *v, int n, Metrics *m) {
    reset_metrics(m); // Zera as métricas antes de começar
    clock_t t0 = clock(); // Marca o tempo inicial de processamento
    fn(v, n, m); // Chama a função de ordenação passada como ponteiro
    clock_t t1 = clock(); // Marca o tempo final de processamento
    // Calcula o tempo decorrido em milissegundos e o retorna
    return 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
}

// --- UTILIDADES ---
void print_array(int *v, int n) { // Função para imprimir os elementos de um array
    for (int i = 0; i < n; i++) printf("%d ", v[i]); // Imprime cada elemento seguido de um espaço
    printf("\n"); // Quebra de linha após a impressão
}

// --- MENU ---
// Função que lida com a execução de um algoritmo de ordenação para um dado vetor e caso
void ordenar_vetor(int *v, int n, const char *caso) {
    Metrics m; // Declara a estrutura para armazenar as métricas
    int op; // Variável para a opção do usuário

    printf("\nEscolha o método:\n"); // Menu de escolha do método de ordenação
    printf("1 - Selection Sort\n");
    printf("2 - Insertion Sort\n");
    printf("3 - Quick Sort\n");
    printf("Opção: ");
    if (scanf("%d", &op) != 1) { // Tenta ler a opção
        int c; while ((c = getchar()) != '\n' && c != EOF) {} // Limpa o buffer em caso de erro de leitura
        printf("Entrada inválida.\n");
        return;
    }

    // Determina o nome do método de ordenação com base na opção
    const char *metodo = (op == 1) ? "selection" :
                         (op == 2) ? "insertion" :
                         (op == 3) ? "quick" : NULL;

    if (!metodo) { // Se a opção for inválida
        printf("Opção inválida.\n");
        return;
    }

    int *aux = malloc(sizeof(int) * n); // Aloca memória para uma cópia do vetor (para não alterar o original)
    if (!aux) { perror("malloc"); return; } // Verifica se a alocação falhou
    for (int i = 0; i < n; i++) aux[i] = v[i]; // Copia o vetor original para o vetor auxiliar

    double tempo; // Variável para armazenar o tempo de execução
    // Chama a função run_sort apropriada com base na opção do usuário
    if (op == 1) tempo = run_sort(selection_sort, aux, n, &m);
    else if (op == 2) tempo = run_sort(insertion_sort, aux, n, &m);
    else tempo = run_sort(quick_sort, aux, n, &m);

    printf("\n--- RESULTADO (%s) ---\n", caso); // Imprime o cabeçalho do resultado
    print_array(aux, n); // Imprime o array ordenado

    printf("\nCSV:\n"); // Imprime o cabeçalho do formato CSV
    printf("metodo,N,caso,comparacoes,trocas,tempo_ms\n");
    // Imprime as métricas no formato CSV
    printf("%s,%d,%s,%lld,%lld,%.3f\n", metodo, n, caso, m.cmp, m.swap, tempo);

    free(aux); // Libera a memória alocada para o vetor auxiliar

    int c; while ((c = getchar()) != '\n' && c != EOF) {} // Limpa o buffer de entrada (após o scanf)
    esperar_enter(); // Pausa para o usuário ver o resultado
}

// --- RGM ---
// Função para ordenar os dígitos do RGM (Registro Geral de Matrícula)
void menu_rgm() {
    char rgm[100]; // Buffer para ler o RGM como string
    printf("Digite seu RGM: ");
    if (scanf("%99s", rgm) != 1) return; // Lê o RGM

    int n = strlen(rgm); // Obtém o número de dígitos (tamanho da string)
    int *v = malloc(sizeof(int) * n); // Aloca memória para o vetor de inteiros
    if (!v) { perror("malloc"); return; } // Verifica a alocação

    // Converte cada caractere dígito do RGM para seu valor inteiro
    for (int i = 0; i < n; i++) v[i] = rgm[i] - '0';

    ordenar_vetor(v, n, "rgm"); // Chama a função para escolher o método e ordenar
    free(v); // Libera a memória alocada
}

// --- BENCHMARK AUTOMÁTICO ---
// Função para realizar um benchmark automático em tamanhos predefinidos
void menu_benchmark() {
    int op; // Variável para a opção do método

    printf("\nEscolha o método para o benchmark:\n"); // Menu de escolha
    printf("1 - Selection Sort\n");
    printf("2 - Insertion Sort\n");
    printf("3 - Quick Sort\n");
    printf("Opção: ");
    scanf("%d", &op); // Lê a opção

    void (*fn)(int*,int,Metrics*) = NULL; // Ponteiro de função para o método de ordenação
    const char *metodo = NULL; // Nome do método

    // Atribui o ponteiro de função e o nome com base na opção
    if (op == 1) fn = selection_sort, metodo = "selection";
    else if (op == 2) fn = insertion_sort, metodo = "insertion";
    else if (op == 3) fn = quick_sort, metodo = "quick";
    else { printf("Opção inválida!\n"); return; } // Trata opção inválida

    int tamanhos[3] = {100, 1000, 10000}; // Tamanhos dos vetores para o benchmark

    printf("\nCSV:\n"); // Imprime o cabeçalho do formato CSV
    printf("metodo,N,caso,comparacoes,trocas,tempo_ms\n");

    for (int t = 0; t < 3; t++) { // Loop para cada tamanho de vetor
        int n = tamanhos[t]; // Tamanho atual do vetor
        int *v = malloc(sizeof(int) * n); // Aloca memória
        if (!v) { perror("malloc"); return; }

        // Preenche o vetor com números aleatórios
        for (int i = 0; i < n; i++) v[i] = rand() % 10000;

        Metrics m; // Estrutura para métricas
        // Executa a ordenação e mede o tempo
        double tempo = run_sort(fn, v, n, &m);

        // Imprime as métricas no formato CSV (o caso é "benchmark")
        printf("%s,%d,benchmark,%lld,%lld,%.3f\n",
               metodo, n, m.cmp, m.swap, tempo);

        free(v); // Libera a memória
    }

    int c; while ((c = getchar()) != '\n' && c != EOF) {} // Limpa o buffer de entrada
    esperar_enter(); // Pausa
}

// --- MAIN ---
int main() {
    srand((unsigned)time(NULL)); // Inicializa o gerador de números aleatórios com base no tempo atual

    while (1) { // Loop principal do menu
        printf("\n=== MENU PRINCIPAL ===\n"); // Imprime o menu principal
        printf("1 - Ordenar RGM:\n");
        printf("2 - Ordenar numeros aleatorios:\n");
        printf("0 - Sair\n");
        printf("Opção: ");

        int op; // Variável para a opção do menu
        if (scanf("%d", &op) != 1) { // Tenta ler a opção
            int c; while ((c = getchar()) != '\n' && c != EOF) {} // Limpa o buffer em caso de entrada inválida
            continue; // Volta para o início do loop
        }

        if (op == 1) menu_rgm(); // Chama a função para ordenar o RGM
        else if (op == 2) menu_benchmark(); // Chama a função de benchmark
        else if (op == 0) break; // Sai do loop e termina o programa
        else printf("Opção inválida!\n"); // Trata opção inválida
    }

    return 0; // Indica que o programa terminou com sucesso
}
