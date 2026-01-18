//TODO: fazer verificação para saber se m está dentro do escopo


#include <iostream>
#include <vector>
#include <list>
#include <queue>   // Para fila de prioridade (Dijkstra)
#include <limits>  // Para usar o "Infinito"

#include "../include/Grafo.hpp"
#include "../include/Leitor.hpp"

using namespace std;

int main() {
    string arquivo = "../data/crd100";
    
    Grafo* g = Leitor::lerInstancia(arquivo);

    // Menu
    int escolha = 1;
    while (escolha != 0){
        int r, alpha, interacoes, block;
        cout << "Escolha o algoritmo a ser executado:\n";
        cout << "1 - Algoritmo Guloso\n";
        cout << "2 - Algoritmo Guloso Randomizado\n";
        cout << "3 - Algorito Guloso Randomizado Reativo\n";
        cout << "0 - Sair\n";

        cin >> escolha;

        switch (escolha)
        {
        case 1:
            g->GA();
            break;
        case 2:
            while (true)
            {
                cout << "Escolha uma porcentagem de 0 até 100:\n";
                cin >> alpha;
                if(alpha >= 0 && alpha <= 100){
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n";
            }
            
            while (true)
            {
                cout << "Escolha o número de iterações:\n";
                cin >> interacoes;
                if(interacoes > 0){
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n";
            }
            g->GRASP(alpha/100, interacoes);
            break;
        case 3:
            while(true){
                cout << "Escolha o nível de aleatoriedade do algorítmo:\n1 - Baixo\n2 - Médio\n3 - Alto\n";
                cin >> escolha;
                if(escolha >= 1 && escolha <= 3){
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n";
            }
            
            while(true){
                cout << "Escolha o número de iterações:\n";
                cin >> interacoes;
                if(interacoes > 0){
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n";
            }
            
            while(true){
                cout << "Escolha o tamanho do bloco de atualização das probabilidades:\n";
                cin >> block;
                if(block > 0 && block <= interacoes){
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n";
            }
            g->GRASPReativo(escolha, interacoes, block);
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