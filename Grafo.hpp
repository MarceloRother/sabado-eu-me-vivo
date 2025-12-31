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

// 1. Estrutura da Aresta
struct Aresta {
    int origem;
    int destino; // O índice do nó para onde essa aresta aponta
    int peso;
    
    // Construtor para facilitar a criação
    Aresta(int u, int v, int p) : origem(u), destino(v), peso(p) {}
};

// 2. Estrutura do Nó
struct No {
    int id;
    string nome;
};

// Comparador para a fila de prioridade (usado em Prim e Kruskal)
struct ComparaPeso {
    bool operator()(const Aresta& a, const Aresta& b){
        return a.peso > b.peso;
    }
};

// 3. A Classe Grafo
class Grafo {
private:
    int numeroDeVertices;
    GrafoTipo tipo;
    
    // O CORAÇÃO DO GRAFO: Lista de Adjacência
    // Um vetor onde cada posição é uma lista de arestas
    // adj[0] = lista de arestas saindo do nó 0
    vector<list<Aresta>> adj; 
    
    // Se quiser guardar nomes dos nós, use um vetor auxiliar:
    // vector<No> dadosDosNos;

public:
    // Construtor
    Grafo(int vertices, GrafoTipo t) : tipo(t) {
        this->numeroDeVertices = vertices;
        adj.resize(vertices); // Prepara o vetor para receber os dados
    }

    // Retorna o número de vértices
    int getNumeroDeVertices(){ return numeroDeVertices; }

    GrafoTipo getTipo(){ return tipo; }

    // Adicionar aresta direcionada (Origem -> Destino)
    void adicionarAresta(int origem, int destino, int peso);

    // 4. Algoritmos de caminho mínimo
    void dijkstra(int inicio);
    void floyd();

    // 5. Algoritmo de busca
    void buscaEmProfundidade(int inicio);
    void kruskal();
    void prim(int r);   
};

#endif GRAFO_HPP