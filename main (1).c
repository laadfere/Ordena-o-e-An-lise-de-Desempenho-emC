#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- MÉTRICAS ---
typedef struct {
    long long cmp;
    long long swap;
} Metrics;

void reset_metrics(Metrics *m) {
    m->cmp = 0;
    m->swap = 0;
}

#define COUNT_CMP(m)  ((m)->cmp++)
#define COUNT_SWAP(m) ((m)->swap++)

void esperar_enter() {
    int c;
    printf("\nPressione ENTER para continuar...");
    while ((c = getchar()) != '\n' && c != EOF) {}
    getchar();
}

// --- SELECTION SORT ---
void selection_sort(int *v, int n, Metrics *m) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            COUNT_CMP(m);
            if (v[j] < v[min]) min = j;
        }
        if (min != i) {
            int t = v[i];
            v[i] = v[min];
            v[min] = t;
            COUNT_SWAP(m);
        }
    }
}

// --- INSERTION SORT ---
void insertion_sort(int *v, int n, Metrics *m) {
    for (int i = 1; i < n; i++) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0) {
            COUNT_CMP(m);
            if (v[j] <= key) break;
            v[j + 1] = v[j];
            COUNT_SWAP(m);
            j--;
        }
        v[j + 1] = key;
    }
}

// --- QUICK SORT ---
int partition(int *v, int low, int high, Metrics *m) {
    int pivot = v[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        COUNT_CMP(m);
        if (v[j] <= pivot) {
            i++;
            int t = v[i];
            v[i] = v[j];
            v[j] = t;
            COUNT_SWAP(m);
        }
    }

    int t = v[i + 1];
    v[i + 1] = v[high];
    v[high] = t;
    COUNT_SWAP(m);

    return i + 1;
}

void quick_sort_rec(int *v, int low, int high, Metrics *m) {
    if (low < high) {
        int p = partition(v, low, high, m);
        quick_sort_rec(v, low, p - 1, m);
        quick_sort_rec(v, p + 1, high, m);
    }
}

void quick_sort(int *v, int n, Metrics *m) {
    quick_sort_rec(v, 0, n - 1, m);
}

// --- RODAR SORT ---
double run_sort(void (*fn)(int*,int,Metrics*), int *v, int n, Metrics *m) {
    reset_metrics(m);
    clock_t t0 = clock();
    fn(v, n, m);
    clock_t t1 = clock();
    return 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
}

// --- UTILIDADES ---
void print_array(int *v, int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

// --- ORDENAR VETOR MANUAL ---
void ordenar_vetor(int *v, int n, const char *caso) {
    Metrics m;
    int op;

    printf("\nEscolha o método:\n");
    printf("1 - Selection Sort\n");
    printf("2 - Insertion Sort\n");
    printf("3 - Quick Sort\n");
    printf("Opção: ");

    if (scanf("%d", &op) != 1) {
        int c; while ((c = getchar()) != '\n');
        printf("Entrada inválida.\n");
        return;
    }

    const char *metodo =
        (op == 1) ? "selection" :
        (op == 2) ? "insertion" :
        (op == 3) ? "quick" : NULL;

    if (!metodo) {
        printf("Opção inválida.\n");
        return;
    }

    int *aux = malloc(sizeof(int) * n);
    if (!aux) { perror("malloc"); return; }
    for (int i = 0; i < n; i++) aux[i] = v[i];

    double tempo;
    if (op == 1) tempo = run_sort(selection_sort, aux, n, &m);
    else if (op == 2) tempo = run_sort(insertion_sort, aux, n, &m);
    else tempo = run_sort(quick_sort, aux, n, &m);

    printf("\n--- RESULTADO (%s) ---\n", caso);
    print_array(aux, n);

    printf("\nCSV:\n");
    printf("metodo,N,caso,comparacoes,trocas,tempo_ms\n");
    printf("%s,%d,%s,%lld,%lld,%.3f\n",
           metodo, n, caso, m.cmp, m.swap, tempo);

    free(aux);

    int c; while ((c = getchar()) != '\n');
    esperar_enter();
}

// --- BENCHMARK AUTOMÁTICO ---
void benchmark() {
    int tamanhos[] = {100, 1000, 10000};
    const char *nomes_metodos[] = {"selection", "insertion", "quick"};
    void (*funcoes[])(int*,int,Metrics*) = {
        selection_sort,
        insertion_sort,
        quick_sort
    };

    printf("\n=== BENCHMARK AUTOMÁTICO ===\n");
    printf("Tamanhos: 100, 1000, 10000\n");
    printf("Métodos: Selection, Insertion, Quick\n\n");

    printf("metodo,N,caso,comparacoes,trocas,tempo_ms\n");

    for (int t = 0; t < 3; t++) {
        int N = tamanhos[t];

        int *v = malloc(sizeof(int) * N);
        int *aux = malloc(sizeof(int) * N);
        if (!v || !aux) { perror("malloc"); return; }

        // gerar caso aleatório
        for (int i = 0; i < N; i++) v[i] = rand() % 100000;

        // rodar 3 métodos
        for (int m = 0; m < 3; m++) {
            Metrics M;
            for (int i = 0; i < N; i++) aux[i] = v[i];

            double tempo = run_sort(funcoes[m], aux, N, &M);

            printf("%s,%d,benchmark,%lld,%lld,%.3f\n",
                   nomes_metodos[m], N, M.cmp, M.swap, tempo);
        }

        free(v);
        free(aux);
    }

    esperar_enter();
}

// --- RGM ---
void menu_rgm() {
    char rgm[100];
    printf("Digite seu RGM: ");
    if (scanf("%99s", rgm) != 1) return;

    int n = strlen(rgm);
    int *v = malloc(sizeof(int) * n);
    if (!v) { perror("malloc"); return; }

    for (int i = 0; i < n; i++) v[i] = rgm[i] - '0';

    ordenar_vetor(v, n, "rgm");
    free(v);
}

// --- MAIN ---
int main() {
    srand((unsigned)time(NULL));

    while (1) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Ordenar RGM\n");
        printf("2 - Ordenar vetor aleatório (10 números)\n");
        printf("3 - Benchmark automático (100, 1000, 10000)\n");
        printf("0 - Sair\n");
        printf("Opção: ");

        int op;
        if (scanf("%d", &op) != 1) {
            int c; while ((c = getchar()) != '\n');
            continue;
        }

        if (op == 1) {
            menu_rgm();

        } else if (op == 2) {
            int v[10];
            for (int i = 0; i < 10; i++) v[i] = rand() % 100;

            printf("Vetor gerado:\n");
            print_array(v, 10);

            ordenar_vetor(v, 10, "aleatorio");

        } else if (op == 3) {
            benchmark();

        } else if (op == 0) break;
        else printf("Opção inválida!\n");
    }

    return 0;
}
