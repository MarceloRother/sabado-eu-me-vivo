#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>
#include <vector>
#include <list>
#include <queue>   // Para fila de prioridade (Dijkstra)
#include <limits>  // Para usar o "Infinito"

using namespace std;

enum GrafoTipo {
    DIRECIONADO,
    NAO_DIRECIONADO
};

// 2. Estrutura do Nó
struct No {
    int id;
    int conexoes;
};

// A Classe Grafo
class Grafo {
private:
    int numeroDeVertices;
    //GrafoTipo tipo;
    
    // O CORAÇÃO DO GRAFO: Matriz de Adjacência
    // Um vetor onde cada posição é um vector contedo o peso das arestas para cada nó
    // adj[0] = lista dos pesos arestas saindo do nó 0
    vector<vector<int>> adj;
    vector<No> nos;
    
    // Se quiser guardar nomes dos nós, use um vetor auxiliar:
    // vector<No> dadosDosNos;

public:
    // Construtor
    Grafo(int vertices){
        adj.resize(vertices); // Prepara o vetor para receber os dados
        this->adj = vector<vector<int>>(vertices, vector<int>(vertices, 0)); // Inicializa com 0 (sem arestas)
        this->nos = vector<No>(vertices);

        this->numeroDeVertices = vertices;
        for(int i = 0; i < vertices; i++){
            nos[i].id = i;
            nos[i].conexoes = 0;
        }
    }

    // Retorna o número de vértices
    int getNumeroDeVertices(){ return numeroDeVertices; }

    //GrafoTipo getTipo(){ return tipo; }

    // Adicionar aresta direcionada (Origem -> Destino)
    void adicionarAresta(int origem, int destino, int peso);

    // IMPLEMENTAÇÕES DOS ALGORITMOS
    void GA();
    void GRASP(int r, int alpha, int interacoes);
    void GRASPReativo(int escolha, int interacoes, int block);
};

#endif GRAFO_HPP