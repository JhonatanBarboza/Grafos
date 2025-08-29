/**
 * @file DFS.h
 * @brief Assinatura das funções de implementação do algoritmo de Busca em Profundidade (DFS).
 */
#ifndef DFS_H
    #define DFS_H

    // inclui as bibliotecas padrão
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    // definição das estruturas de dados
    typedef struct Pilha_ Pilha;
    typedef struct No_ No;
    typedef struct Grafo_ Grafo;
    typedef struct ResultadoBusca_ ResultadoBusca;
    typedef enum TipoAresta_ TipoAresta;

    // =============================================================================
    // Definição das funções
    // =============================================================================

    // Operações da pilha
    Pilha* criarPilha();
    bool pilhaVazia(Pilha* pilha);
    bool pilhaCheia(Pilha* pilha);
    bool empilhar(Pilha* pilha, int elemento);
    int desempilhar(Pilha* pilha);
    int consultarTopo(Pilha* pilha);
    void destruirPilha(Pilha* pilha);

    // Operações do grafo
    Grafo* criarGrafo(int numeroVertices);
    No* criarNo(int vertice);
    bool adicionarAresta(Grafo* grafo, int origem, int destino);
    bool adicionarArestaBidirecional(Grafo* grafo, int vertice1, int vertice2);
    void reiniciarBusca(Grafo* grafo);
    bool executarDFSRecursivo(Grafo* grafo, int verticeInicial);
    bool executarDFSIterativo(Grafo* grafo, int verticeInicial);

    // Algoritmo de Busca em Profundidade (DFS) recursivo
    void dfsRecursivoAux(Grafo* grafo, int vertice);
    bool executarDFSRecursivo(Grafo* grafo, int verticeInicial);

    // Algoritmo de Busca em Profundidade (DFS) iterativo
    bool executarDFSIterativo(Grafo* grafo, int verticeInicial);

    // DFS completo
    void executarDFSCompleto(Grafo* grafo);

    // Busca de caminho específico
    bool buscarCaminhoAux(Grafo* grafo, int atual, int destino, int* caminho, int* tamanho) {
    ResultadoBusca buscarCaminho(Grafo* grafo, int origem, int destino);

    // Detecção de ciclos 
    bool detectarCicloNaoDirigidoAux(Grafo* grafo, int vertice, int pai);
    bool temCiclo(Grafo* grafo);

#endif