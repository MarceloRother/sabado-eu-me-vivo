#include <iostream>
#include <vector>
#include <list>
#include <queue>   // Para fila de prioridade (Dijkstra)
#include <limits>  // Para usar o "Infinito"

#include "Grafo.hpp"

using namespace std;

int main() {
    // Criando um grafo com 4 nós (0 a 3)
    Grafo g(4, DIRECIONADO);

    // Conectando (como se fossem cidades e distancias)
    g.adicionarAresta(0, 1, 2);
    g.adicionarAresta(0, 2, 8);
    g.adicionarAresta(1, 2, 3);
    g.adicionarAresta(1, 3, 5);
    g.adicionarAresta(2, 3, 4);

    // Menu
    int escolha = 1;
    while (escolha != 0){
        cout << "Escolha o algoritmo a ser executado:\n";
        cout << "1 - Dijkstra\n";
        cout << "2 - Floyd-Warshall\n";
        cout << "3 - Prim\n";
        cout << "0 - Sair\n";

        cin >> escolha;

        switch (escolha)
        {
        case 1:
            cout << "Escolha um nó de partida de 0 a "<< g.getNumeroDeVertices() - 1 << ":\n";
            int n;
            cin >> n;
            g.dijkstra(n);
            break;
        case 2:
            g.floyd();
            break;
        case 3:
            cout << "Escolha um nó de partida de 0 a "<< g.getNumeroDeVertices() - 1 << ":\n";
            int m;
            cin >> m;
            g.prim(m);
            break;
        case 0:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opção inválida. Tente novamente.\n";
            break;
        }

    }

    return 0;
}