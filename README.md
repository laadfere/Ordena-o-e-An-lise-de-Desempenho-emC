README: Análise de Algoritmos de Ordenação em C
Olá! Este é um programa em C projetado para demonstrar, executar e analisar o desempenho de três algoritmos de ordenação fundamentais: Selection Sort, Insertion Sort e Quick Sort.
O programa mede o tempo de execução e contabiliza duas métricas essenciais de eficiência: o número de comparações e o número de trocas realizadas. Os resultados são exibidos de forma clara e também em formato CSV para facilitar a análise de dados.

Funcionalidades Principais
O programa apresenta um menu principal com duas opções de execução:
1. Ordenar RGM (Registro Geral de Matrícula)
Permite ao usuário inserir uma sequência de dígitos (simulando um RGM).
Os dígitos são convertidos em um vetor de inteiros.
O usuário escolhe qual dos três métodos de ordenação aplicar.
Saída: Exibe o vetor ordenado e um resumo completo das métricas (comparações, trocas e tempo) para esse caso específico.
2. Entradas B (Benchmark Automático)
Realiza um teste de desempenho em três tamanhos fixos de vetor: N=100, N=1000 e N=10000.
O usuário escolhe qual algoritmo (Selection, Insertion ou Quick Sort) rodar nos testes.
Os vetores são preenchidos com números aleatórios.
Saída: Gera uma tabela de resultados em formato CSV no terminal, contendo as métricas para cada um dos três tamanhos, ideal para análise de eficiência em diferentes escalas.

Algoritmos Implementados
Insertion Sort
Constrói a lista ordenada inserindo repetidamente um novo elemento na posição correta da sub-lista já ordenada.




Quick Sort
Divide e Conquista. Particiona o array em torno de um pivô e ordena recursivamente as sub-listas.







Estrutura e Métricas
O código utiliza uma estrutura Metrics para rastrear o desempenho:
COUNT_CMP(m): Macro que incrementa o contador de comparações.
COUNT_SWAP(m): Macro que incrementa o contador de trocas (movimentações).
run_sort(...): Função central que executa o algoritmo, mede o tempo (em milissegundos) usando clock() e armazena as métricas.

Como Compilar e Executar
Salvar: Salve o código em um arquivo, por exemplo, sort_analyzer.c.
      2. Compilar (GCC): Abra o terminal/prompt de comando e use o compilador GCC:
          Bash
         gcc sort_analyzer.c -o sort_analyzer
      3. Executar:
No Linux/macOS: ./sort_analyzer
No Windows: sort_analyzer.exe
Ao executar, o programa apresentará o Menu Principal, permitindo a escolha entre ordenar seu RGM ou iniciar um dos benchmarks automáticos.
Ordenar RGM (Registro Geral de Matrícula)
Esta opção é ideal para testar os algoritmos de ordenação em um vetor de tamanho pequeno e fixo (igual ao número de dígitos do seu RGM), simulando um caso de uso prático para a disciplina.
 Entradas B (Benchmark Automático)
Esta opção é crucial para realizar uma análise de complexidade ao testar como os algoritmos se comportam quando o tamanho da entrada (N) cresce. Os testes são feitos com vetores preenchidos com números aleatórios (rand()).
Fluxo de Execução:
O usuário escolhe um único método para o benchmark (ex: Quick Sort).
O programa roda esse método três vezes, em vetores de tamanhos crescentes e pré-definidos:
N=100
N=1000
N=10000
Cada execução gera uma linha de dados.
Saída de Dados: O resultado final é uma tabela CSV com três linhas, facilitando a comparação das métricas à medida que N aumenta, permitindo visualizar a diferença entre complexidades O(N2) e O(NlogN).

Sair
Finaliza a execução do programa.
Escolhendo o Melhor Método

Análise de Desempenho e Conclusão
A performance foi avaliada considerando a correção, a robustez em diferentes cenários e a escalabilidade (crescimento das métricas com N).
1. Análise para o RGM (45663734, N=8)
Para entradas muito pequenas, como RGM, as diferenças de tempo de execução são insignificantes. O foco se torna a eficiência em termos de passos (Comparações e Trocas):
Algoritmo
Complexidade de Comparações
Comparações Esperadas (N=8)
Trocas (Pior Caso)
Selection Sort
O(N2) (Sempre)
2N(N−1)​=28×7​=28
7
Insertion Sort
O(N) a O(N2)
Depende da ordem (próximo de O(N2))
O(N2)
Quick Sort
O(NlogN) a O(N2)
Variável (baixo)
Variável (baixo)


Observação Prática (RGM): O Selection Sort é o que possui o número de comparações mais previsível (sempre 28). O Insertion Sort terá muitas trocas, pois o array inicial (4 5 6 6 3 7 3 4) está bastante desordenado, mas ainda assim performa bem devido ao N ser minúsculo.
2. Escalabilidade: Conexão Teoria e Prática (Benchmark)
O verdadeiro valor de um algoritmo se manifesta no Benchmark Automático (Entradas B), onde N atinge 1000 e 10000.
2.1. Algoritmos de Crescimento Quadrático (O(N2))
O Selection Sort e o Insertion Sort pertencem a esta classe.
Teoria: O número de comparações cresce com o quadrado de N. Se N for multiplicado por 10 (de 1.000 para 10.000), o número de comparações e o tempo de execução aumentarão por um fator de ≈100.
Prática Esperada: O seu resultado CSV demonstrará claramente que o tempo gasto em N=10000 será dramaticamente maior (cerca de 100 vezes) do que em N=100.
2.2. Algoritmo de Crescimento Log-Linear (O(NlogN))
O Quick Sort pertence a esta classe no caso médio, que é o caso da entrada aleatória do benchmark.
Teoria: O crescimento é muito mais suave. Quando N é multiplicado por 10 (de 1.000 para 10.000), o número de operações (e o tempo) deve aumentar por um fator de ≈10×logNantigo​logNnovo​​, que é um fator muito menor que 100.
Prática Esperada: O seu resultado CSV confirmará que o Quick Sort será o mais rápido em N=10000, apresentando o menor aumento no tempo em comparação com os outros dois métodos.
3. Considerações de Robustez e Estabilidade
Algoritmo
Estabilidade
Uso de Memória (Espaço)
Sensibilidade ao Caso de Teste
Selection Sort
Instável
O(1) (In-place)
Baixa. Número de Comparações é constante.
Insertion Sort
Estável
O(1) (In-place)
Alta. Excepcional para dados quase ordenados (O(N)).
Quick Sort
Instável
O(logN) (Pilha de Recursão)
Alta. Vulnerável ao pior caso (O(N2)) com pivôs ruins.

Exportar para as Planilhas
Conclusão
A análise do seu código e a teoria por trás dele levam à seguinte conclusão, validada pelos resultados esperados do benchmark:
O Quick Sort é inequivocamente o método superior para lidar com grandes volumes de dados (alta escalabilidade), pois seu tempo de execução escala de forma muito mais eficiente (O(NlogN)) do que os métodos quadráticos.
Melhor para Benchmarks (N grande): Quick Sort.
Melhor para arrays quase ordenados: Insertion Sort.
Melhor em eficiência de trocas (N pequeno): O Selection Sort garante o menor número de trocas, sendo útil em cenários onde escrever na memória é muito caro, embora seu tempo O(N2) o desqualifique para N grande.
Para uma aplicação geral e robusta, onde a velocidade em grandes bases de dados é crítica, o Quick Sort é o vencedor.




