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

void esperar_enter() {
    int c;
    printf("\nPressione ENTER para continuar...");
    while ((c = getchar()) != '\n' && c != EOF) {}
    getchar();
}

#define COUNT_CMP(m)  ((m)->cmp++)
#define COUNT_SWAP(m) ((m)->swap++)

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

// --- RODAR SORT ---
double run_sort(void (*fn)(int*,int,Metrics*), int *v, int n, Metrics *m) {
    reset_metrics(m);
    clock_t t0 = clock();
    fn(v, n, m);
    clock_t t1 = clock();
    return 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;
}

void print_array(int *v, int n) {
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

// --- MENU PARA ORDENAR ---
void ordenar_vetor(int *v, int n, const char *caso) {
    Metrics m;

    printf("\nMétodo disponível:\n");
    printf("1 - Insertion Sort\n");
    printf("Opção: ");

    int op;
    if (scanf("%d", &op) != 1 || op != 1) {
        printf("Opção inválida.\n");
        return;
    }

    int *aux = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) aux[i] = v[i];

    double tempo = run_sort(insertion_sort, aux, n, &m);

    printf("\n--- RESULTADO (%s) ---\n", caso);
    print_array(aux, n);

    printf("\nCSV:\n");
    printf("metodo,N,caso,comparacoes,trocas,tempo_ms\n");
    printf("insertion,%d,%s,%lld,%lld,%.3f\n", n, caso, m.cmp, m.swap, tempo);

    free(aux);

    int c; while ((c = getchar()) != '\n' && c != EOF) {}
    esperar_enter();
}

// --- RGM ---
void menu_rgm() {
    char rgm[100];
    printf("Digite seu RGM: ");
    scanf("%99s", rgm);

    int n = strlen(rgm);
    int *v = malloc(sizeof(int) * n);

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
        printf("0 - Sair\n");
        printf("Opção: ");

        int op;
        if (scanf("%d", &op) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        if (op == 1) menu_rgm();
        else if (op == 0) break;
        else printf("Opção inválida!\n");
    }

    return 0;
}
