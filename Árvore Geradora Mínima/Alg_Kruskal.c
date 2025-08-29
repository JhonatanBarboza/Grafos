/*
 * Algoritmo de Kruskal - Árvore Geradora Mínima (AGM)
 * Implementação completa em português
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_ARESTAS 1000

// =============================================================================
// DEFINIÇÃO DOS TIPOS DE DADOS
// =============================================================================

// Estrutura para representar uma aresta
typedef struct {
    int origem;
    int destino;
    int peso;
} Aresta;

// Estrutura para representar o grafo
typedef struct {
    int numeroVertices;
    int numeroArestas;
    Aresta arestas[MAX_ARESTAS];
} Grafo;

// Estrutura para Union-Find (Disjoint Set)
typedef struct {
    int pai[MAX_VERTICES];
    int rank[MAX_VERTICES];
    int numeroConjuntos;
} UnionFind;

// Estrutura para resultado da AGM
typedef struct {
    Aresta* arestas;
    int numeroArestas;
    int pesoTotal;
    bool sucesso;
} ResultadoAGM;

// =============================================================================
// OPERAÇÕES DO UNION-FIND
// =============================================================================

// Inicializa a estrutura Union-Find
UnionFind* criarUnionFind(int numeroVertices) {
    if (numeroVertices <= 0 || numeroVertices > MAX_VERTICES) {
        printf("ERRO: Número de vértices inválido para Union-Find!\n");
        return NULL;
    }
    
    UnionFind* uf = (UnionFind*)malloc(sizeof(UnionFind));
    if (uf == NULL) return NULL;
    
    uf->numeroConjuntos = numeroVertices;
    
    // Cada vértice começa como seu próprio pai (conjunto individual)
    for (int i = 0; i < numeroVertices; i++) {
        uf->pai[i] = i;
        uf->rank[i] = 0;
    }
    
    return uf;
}

// Encontra o representante do conjunto (com compressão de caminho)
int encontrar(UnionFind* uf, int vertice) {
    if (uf->pai[vertice] != vertice) {
        // Compressão de caminho: faz todos os nós apontarem diretamente para a raiz
        uf->pai[vertice] = encontrar(uf, uf->pai[vertice]);
    }
    return uf->pai[vertice];
}

// Une dois conjuntos (com união por rank)
bool unir(UnionFind* uf, int vertice1, int vertice2) {
    int raiz1 = encontrar(uf, vertice1);
    int raiz2 = encontrar(uf, vertice2);
    
    // Se já estão no mesmo conjunto, não há nada a fazer
    if (raiz1 == raiz2) {
        return false;
    }
    
    // União por rank: anexa a árvore menor à maior
    if (uf->rank[raiz1] < uf->rank[raiz2]) {
        uf->pai[raiz1] = raiz2;
    } else if (uf->rank[raiz1] > uf->rank[raiz2]) {
        uf->pai[raiz2] = raiz1;
    } else {
        uf->pai[raiz2] = raiz1;
        uf->rank[raiz1]++;
    }
    
    uf->numeroConjuntos--;
    return true;
}

// Verifica se dois vértices estão no mesmo conjunto
bool estaoConectados(UnionFind* uf, int vertice1, int vertice2) {
    return encontrar(uf, vertice1) == encontrar(uf, vertice2);
}

// Libera memória do Union-Find
void destruirUnionFind(UnionFind* uf) {
    if (uf != NULL) {
        free(uf);
    }
}

// =============================================================================
// OPERAÇÕES DO GRAFO
// =============================================================================

// Cria um novo grafo
Grafo* criarGrafo(int numeroVertices) {
    if (numeroVertices <= 0 || numeroVertices > MAX_VERTICES) {
        printf("ERRO: Número de vértices inválido!\n");
        return NULL;
    }
    
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) return NULL;
    
    grafo->numeroVertices = numeroVertices;
    grafo->numeroArestas = 0;
    
    return grafo;
}

// Adiciona uma aresta ao grafo
bool adicionarAresta(Grafo* grafo, int origem, int destino, int peso) {
    if (grafo == NULL || origem < 0 || destino < 0 || 
        origem >= grafo->numeroVertices || destino >= grafo->numeroVertices ||
        grafo->numeroArestas >= MAX_ARESTAS) {
        printf("ERRO: Parâmetros inválidos para adicionar aresta!\n");
        return false;
    }
    
    if (origem == destino) {
        printf("AVISO: Ignorando aresta de um vértice para ele mesmo!\n");
        return false;
    }
    
    Aresta* aresta = &grafo->arestas[grafo->numeroArestas];
    aresta->origem = origem;
    aresta->destino = destino;
    aresta->peso = peso;
    
    grafo->numeroArestas++;
    return true;
}

// Libera memória do grafo
void destruirGrafo(Grafo* grafo) {
    if (grafo != NULL) {
        free(grafo);
    }
}

// =============================================================================
// ALGORITMOS DE ORDENAÇÃO
// =============================================================================

// Função auxiliar para trocar duas arestas
void trocarArestas(Aresta* a, Aresta* b) {
    Aresta temp = *a;
    *a = *b;
    *b = temp;
}

// Partição para o Quick Sort
int particionar(Aresta arr[], int baixo, int alto) {
    int pivo = arr[alto].peso;
    int i = (baixo - 1);
    
    for (int j = baixo; j <= alto - 1; j++) {
        if (arr[j].peso <= pivo) {
            i++;
            trocarArestas(&arr[i], &arr[j]);
        }
    }
    
    trocarArestas(&arr[i + 1], &arr[alto]);
    return (i + 1);
}

// Quick Sort para ordenar arestas por peso
void quickSort(Aresta arr[], int baixo, int alto) {
    if (baixo < alto) {
        int pi = particionar(arr, baixo, alto);
        
        quickSort(arr, baixo, pi - 1);
        quickSort(arr, pi + 1, alto);
    }
}

// Ordena as arestas do grafo por peso crescente
void ordenarArestasPorPeso(Grafo* grafo) {
    if (grafo != NULL && grafo->numeroArestas > 1) {
        quickSort(grafo->arestas, 0, grafo->numeroArestas - 1);
    }
}

// =============================================================================
// ALGORITMO DE KRUSKAL
// =============================================================================

// Implementa o algoritmo de Kruskal para encontrar a AGM
ResultadoAGM kruskal(Grafo* grafo) {
    ResultadoAGM resultado = {NULL, 0, 0, false};
    
    if (grafo == NULL || grafo->numeroVertices <= 0) {
        printf("ERRO: Grafo inválido!\n");
        return resultado;
    }
    
    printf("=== EXECUTANDO ALGORITMO DE KRUSKAL ===\n");
    printf("Grafo com %d vértices e %d arestas\n", 
           grafo->numeroVertices, grafo->numeroArestas);
    
    // Alocar memória para a AGM
    int maxArestasAGM = grafo->numeroVertices - 1;
    resultado.arestas = (Aresta*)malloc(maxArestasAGM * sizeof(Aresta));
    if (resultado.arestas == NULL) {
        printf("ERRO: Falha na alocação de memória!\n");
        return resultado;
    }
    
    // Ordenar arestas por peso crescente
    printf("Ordenando arestas por peso...\n");
    ordenarArestasPorPeso(grafo);
    
    // Inicializar Union-Find
    UnionFind* uf = criarUnionFind(grafo->numeroVertices);
    if (uf == NULL) {
        free(resultado.arestas);
        resultado.arestas = NULL;
        return resultado;
    }
    
    printf("\nProcessando arestas:\n");
    printf("%-8s %-8s %-8s %-15s\n", "Origem", "Destino", "Peso", "Status");
    printf("----------------------------------------\n");
    
    // Processar cada aresta em ordem crescente de peso
    for (int i = 0; i < grafo->numeroArestas && resultado.numeroArestas < maxArestasAGM; i++) {
        Aresta arestaAtual = grafo->arestas[i];
        
        // Verificar se adicionar esta aresta criaria um ciclo
        if (!estaoConectados(uf, arestaAtual.origem, arestaAtual.destino)) {
            // Adicionar aresta à AGM
            resultado.arestas[resultado.numeroArestas] = arestaAtual;
            resultado.numeroArestas++;
            resultado.pesoTotal += arestaAtual.peso;
            
            // Unir os conjuntos
            unir(uf, arestaAtual.origem, arestaAtual.destino);
            
            printf("%-8d %-8d %-8d %-15s\n", 
                   arestaAtual.origem, arestaAtual.destino, arestaAtual.peso, "ACEITA");
        } else {
            printf("%-8d %-8d %-8d %-15s\n", 
                   arestaAtual.origem, arestaAtual.destino, arestaAtual.peso, "REJEITADA (ciclo)");
        }
    }
    
    // Verificar se o grafo é conexo
    if (uf->numeroConjuntos == 1) {
        resultado.sucesso = true;
        printf("\n✓ AGM encontrada com sucesso!\n");
        printf("Número de arestas na AGM: %d\n", resultado.numeroArestas);
        printf("Peso total da AGM: %d\n", resultado.pesoTotal);
    } else {
        printf("\n✗ Grafo não é conexo!\n");
        printf("Número de componentes: %d\n", uf->numeroConjuntos);
        printf("AGM não pode ser construída.\n");
    }
    
    destruirUnionFind(uf);
    return resultado;
}

// =============================================================================
// VALIDAÇÃO E VERIFICAÇÃO
// =============================================================================

// Verifica se o grafo é conexo (necessário para ter AGM)
bool grafoEConexo(Grafo* grafo) {
    if (grafo == NULL || grafo->numeroVertices <= 1) {
        return true;
    }
    
    UnionFind* uf = criarUnionFind(grafo->numeroVertices);
    if (uf == NULL) return false;
    
    // Processar todas as arestas
    for (int i = 0; i < grafo->numeroArestas; i++) {
        unir(uf, grafo->arestas[i].origem, grafo->arestas[i].destino);
    }
    
    bool conexo = (uf->numeroConjuntos == 1);
    destruirUnionFind(uf);
    
    return conexo;
}

// Verifica se uma solução é uma árvore geradora válida
bool validarAGM(Grafo* grafoOriginal, ResultadoAGM agm) {
    if (!agm.sucesso || agm.arestas == NULL) {
        return false;
    }
    
    // Uma árvore geradora deve ter exatamente n-1 arestas
    int arestasEsperadas = grafoOriginal->numeroVertices - 1;
    if (agm.numeroArestas != arestasEsperadas) {
        printf("ERRO: Número incorreto de arestas na AGM!\n");
        printf("Esperado: %d, Encontrado: %d\n", arestasEsperadas, agm.numeroArestas);
        return false;
    }
    
    // Verificar se todas as arestas da AGM existem no grafo original
    for (int i = 0; i < agm.numeroArestas; i++) {
        bool encontrada = false;
        Aresta arestaAGM = agm.arestas[i];
        
        for (int j = 0; j < grafoOriginal->numeroArestas; j++) {
            Aresta arestaOriginal = grafoOriginal->arestas[j];
            
            if ((arestaAGM.origem == arestaOriginal.origem && 
                 arestaAGM.destino == arestaOriginal.destino) ||
                (arestaAGM.origem == arestaOriginal.destino && 
                 arestaAGM.destino == arestaOriginal.origem)) {
                
                if (arestaAGM.peso == arestaOriginal.peso) {
                    encontrada = true;
                    break;
                }
            }
        }
        
        if (!encontrada) {
            printf("ERRO: Aresta (%d,%d) da AGM não existe no grafo original!\n", 
                   arestaAGM.origem, arestaAGM.destino);
            return false;
        }
    }
    
    // Verificar se a AGM conecta todos os vértices (sem ciclos)
    UnionFind* uf = criarUnionFind(grafoOriginal->numeroVertices);
    if (uf == NULL) return false;
    
    for (int i = 0; i < agm.numeroArestas; i++) {
        if (estaoConectados(uf, agm.arestas[i].origem, agm.arestas[i].destino)) {
            printf("ERRO: AGM contém ciclo!\n");
            destruirUnionFind(uf);
            return false;
        }
        unir(uf, agm.arestas[i].origem, agm.arestas[i].destino);
    }
    
    bool valida = (uf->numeroConjuntos == 1);
    destruirUnionFind(uf);
    
    return valida;
}

// =============================================================================
// FUNÇÕES DE EXIBIÇÃO
// =============================================================================

// Exibe o grafo
void exibirGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        printf("Grafo inválido!\n");
        return;
    }
    
    printf("\n=== REPRESENTAÇÃO DO GRAFO ===\n");
    printf("Vértices: %d, Arestas: %d\n", grafo->numeroVertices, grafo->numeroArestas);
    printf("%-8s %-8s %-8s\n", "Origem", "Destino", "Peso");
    printf("-------------------------\n");
    
    for (int i = 0; i < grafo->numeroArestas; i++) {
        Aresta aresta = grafo->arestas[i];
        printf("%-8d %-8d %-8d\n", aresta.origem, aresta.destino, aresta.peso);
    }
    printf("\n");
}

// Exibe o resultado da AGM
void exibirResultadoAGM(ResultadoAGM agm) {
    printf("\n=== RESULTADO DA ÁRVORE GERADORA MÍNIMA ===\n");
    
    if (!agm.sucesso) {
        printf("❌ AGM não foi encontrada (grafo pode não ser conexo)\n");
        return;
    }
    
    printf("✓ AGM encontrada com sucesso!\n");
    printf("Número de arestas: %d\n", agm.numeroArestas);
    printf("Peso total: %d\n\n", agm.pesoTotal);
    
    printf("Arestas da AGM:\n");
    printf("%-8s %-8s %-8s\n", "Origem", "Destino", "Peso");
    printf("-------------------------\n");
    
    for (int i = 0; i < agm.numeroArestas; i++) {
        Aresta aresta = agm.arestas[i];
        printf("%-8d %-8d %-8d\n", aresta.origem, aresta.destino, aresta.peso);
    }
    printf("\n");
}

// Exibe estatísticas da execução
void exibirEstatisticas(Grafo* grafo, ResultadoAGM agm) {
    if (grafo == NULL) return;
    
    printf("=== ESTATÍSTICAS ===\n");
    printf("Vértices do grafo original: %d\n", grafo->numeroVertices);
    printf("Arestas do grafo original: %d\n", grafo->numeroArestas);
    
    if (agm.sucesso) {
        printf("Arestas na AGM: %d\n", agm.numeroArestas);
        printf("Peso total da AGM: %d\n", agm.pesoTotal);
        printf("Arestas removidas: %d\n", grafo->numeroArestas - agm.numeroArestas);
        
        // Calcular peso total do grafo original
        int pesoTotal = 0;
        for (int i = 0; i < grafo->numeroArestas; i++) {
            pesoTotal += grafo->arestas[i].peso;
        }
        
        printf("Peso total original: %d\n", pesoTotal);
        printf("Economia de peso: %d (%.1f%%)\n", 
               pesoTotal - agm.pesoTotal,
               ((double)(pesoTotal - agm.pesoTotal) / pesoTotal) * 100);
    } else {
        printf("AGM: Não encontrada\n");
    }
    printf("\n");
}

// Libera memória do resultado da AGM
void liberarResultadoAGM(ResultadoAGM* agm) {
    if (agm->arestas != NULL) {
        free(agm->arestas);
        agm->arestas = NULL;
    }
    agm->numeroArestas = 0;
    agm->pesoTotal = 0;
    agm->sucesso = false;
}

// =============================================================================
// FUNÇÃO PRINCIPAL - EXEMPLO DE USO
// =============================================================================

int main() {
    printf("=== ALGORITMO DE KRUSKAL - DEMONSTRAÇÃO ===\n");
    
    // Exemplo 1: Grafo conexo básico
    printf("\n### EXEMPLO 1: GRAFO CONEXO ###\n");
    Grafo* grafo1 = criarGrafo(6);
    
    // Adicionar arestas (grafo não-dirigido, então adicionamos uma vez só)
    adicionarAresta(grafo1, 0, 1, 4);
    adicionarAresta(grafo1, 0, 2, 3);
    adicionarAresta(grafo1, 1, 2, 1);
    adicionarAresta(grafo1, 1, 3, 2);
    adicionarAresta(grafo1, 2, 3, 4);
    adicionarAresta(grafo1, 2, 4, 2);
    adicionarAresta(grafo1, 3, 4, 6);
    adicionarAresta(grafo1, 3, 5, 1);
    adicionarAresta(grafo1, 4, 5, 3);
    
    exibirGrafo(grafo1);
    
    printf("Verificando conectividade: ");
    if (grafoEConexo(grafo1)) {
        printf("✓ Grafo é conexo\n");
    } else {
        printf("✗ Grafo não é conexo\n");
    }
    
    ResultadoAGM agm1 = kruskal(grafo1);
    exibirResultadoAGM(agm1);
    
    printf("Validação da AGM: ");
    if (validarAGM(grafo1, agm1)) {
        printf("✓ AGM válida\n");
    } else {
        printf("✗ AGM inválida\n");
    }
    
    exibirEstatisticas(grafo1, agm1);
    
    // Exemplo 2: Grafo desconexo
    printf("\n### EXEMPLO 2: GRAFO DESCONEXO ###\n");
    Grafo* grafo2 = criarGrafo(5);
    
    // Dois componentes separados
    adicionarAresta(grafo2, 0, 1, 1);
    adicionarAresta(grafo2, 1, 2, 2);
    adicionarAresta(grafo2, 3, 4, 3);  // Componente separado
    
    exibirGrafo(grafo2);
    
    printf("Verificando conectividade: ");
    if (grafoEConexo(grafo2)) {
        printf("✓ Grafo é conexo\n");
    } else {
        printf("✗ Grafo não é conexo\n");
    }
    
    ResultadoAGM agm2 = kruskal(grafo2);
    exibirResultadoAGM(agm2);
    
    // Exemplo 3: Grafo denso
    printf("\n### EXEMPLO 3: GRAFO DENSO ###\n");
    Grafo* grafo3 = criarGrafo(4);
    
    // Grafo completo com 4 vértices
    adicionarAresta(grafo3, 0, 1, 10);
    adicionarAresta(grafo3, 0, 2, 6);
    adicionarAresta(grafo3, 0, 3, 5);
    adicionarAresta(grafo3, 1, 2, 4);
    adicionarAresta(grafo3, 1, 3, 15);
    adicionarAresta(grafo3, 2, 3, 20);
    
    exibirGrafo(grafo3);
    
    ResultadoAGM agm3 = kruskal(grafo3);
    exibirResultadoAGM(agm3);
    exibirEstatisticas(grafo3, agm3);
    
    // Limpeza de memória
    destruirGrafo(grafo1);
    destruirGrafo(grafo2);
    destruirGrafo(grafo3);
    
    liberarResultadoAGM(&agm1);
    liberarResultadoAGM(&agm2);
    liberarResultadoAGM(&agm3);
    
    printf("Programa finalizado com sucesso!\n");
    return 0;
}