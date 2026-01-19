#include <iostream>
#include <vector>
#include <list>
#include <queue>   // Para fila de prioridade (Dijkstra)
#include <limits>  // Para usar o "Infinito"
#include <fstream>
#include <chrono>
#include <ctime>   // Necessário para time()
#include <cstdlib> // Necessário para srand() e atoi()

#include "../include/Grafo.hpp"
#include "../include/Leitor.hpp"

using namespace std;

void salvarResultadosCSV(const string& algoritmo, int melhorCusto, int iteracaoMelhor, float alpha, long long tempoExecucaoMs) {
    // Verifica se o arquivo existe para adicionar cabeçalho na primeira vez
    ifstream verifica("../resultados.csv");
    bool arquivoExiste = verifica.good();
    verifica.close();
    
    ofstream arquivo("../resultados.csv", ios::app);
    if (arquivo.is_open()) {
        // Adiciona cabeçalho se o arquivo não existia
        if (!arquivoExiste) {
            arquivo << "Algoritmo,MelhorCusto,IteracaoMelhor,Alpha,TempoExecucaoMs\n";
        }
        arquivo << algoritmo << "," << melhorCusto << "," << iteracaoMelhor << "," << alpha << "," << tempoExecucaoMs << "\n";
        arquivo.close();
    }
}

int main(int argc, char* argv[]) {
    string arquivo = "../data/crd100";

    unsigned int seed;

    srand(seed);

    Grafo* g = Leitor::lerInstancia(arquivo);

    // Menu
    int escolha = 1;
    while (escolha != 0){
        int r, alpha, interacoes, block, d;
        cout << "Escolha o algoritmo a ser executado:\n";
        cout << "1 - Algoritmo Guloso\n";
        cout << "2 - Algoritmo Guloso Randomizado\n";
        cout << "3 - Algorito Guloso Randomizado Reativo\n";
        cout << "0 - Sair\n";

        cin >> escolha;

        switch (escolha)
        {
        case 1:
            while (true)
            {
                cout << "Defina a restrição de conexões por nó!\n";
                cout << "Deve ser entre 1 e " << g->getNumeroDeVertices() - 1 << ":\n";
                cin >> d;
                if(d >= 1 && d <= g->getNumeroDeVertices() - 1){
                    g->setRestricaoConexoes(d);
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n\n";
            }
            {
                auto inicio = chrono::high_resolution_clock::now();
                ResultadoAlgoritmo resultado = g->GA(d);
                auto fim = chrono::high_resolution_clock::now();
                auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();
                salvarResultadosCSV("guloso", resultado.melhorCusto, resultado.iteracaoMelhor, resultado.alpha, duracao);
            }
            break;
        case 2:
            while (true)
            {
                cout << "Escolha uma porcentagem de 0 até 100:\n";
                cin >> alpha;
                if(alpha >= 0 && alpha <= 100){
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n\n";
            }
            
            while (true)
            {
                cout << "Escolha o número de iterações:\n";
                cin >> interacoes;
                if(interacoes > 0){
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n\n";
            }

            while (true)
            {
                cout << "Defina a restrição de conexões por nó!\n";
                cout << "Deve ser entre 1 e " << g->getNumeroDeVertices() - 1 << ":\n";
                cin >> d;
                if(d >= 1 && d <= g->getNumeroDeVertices() - 1){
                    g->setRestricaoConexoes(d);
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n\n";
            }
            while(true)
            {
                cout << "Digite a seed para o gerador de números aleatórios\n Caso queira uma seed baseada no tempo, digite 0:\n";
                cin >> seed;
                if(seed == 0){
                    seed = static_cast<unsigned int>(time(0));
                }
                srand(seed);
                break;
            }
            {
                auto inicio = chrono::high_resolution_clock::now();
                ResultadoAlgoritmo resultado = g->GRASP(alpha/100.0, interacoes, d);
                auto fim = chrono::high_resolution_clock::now();
                auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();
                salvarResultadosCSV("grasp", resultado.melhorCusto, resultado.iteracaoMelhor, resultado.alpha, duracao);
            }
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

            while (true)
            {
                cout << "Defina a restrição de conexões por nó!\n";
                cout << "Deve ser entre 1 e " << g->getNumeroDeVertices() - 1 << ":\n";
                cin >> d;
                if(d >= 1 && d <= g->getNumeroDeVertices() - 1){
                    g->setRestricaoConexoes(d);
                    break;
                }
                cout << "Valor inválido. Tente novamente.\n\n";
            }
            while(true)
            {
                cout << "Digite a seed para o gerador de números aleatórios\n Caso queira uma seed baseada no tempo, digite 0:\n";
                cin >> seed;
                if(seed == 0){
                    seed = static_cast<unsigned int>(time(0));
                }
                srand(seed);
                break;
            }
            {
                auto inicio = chrono::high_resolution_clock::now();
                ResultadoAlgoritmo resultado = g->GRASPReativo(escolha, interacoes, block, d, seed);
                auto fim = chrono::high_resolution_clock::now();
                auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();
                salvarResultadosCSV("grasp_reativo", resultado.melhorCusto, resultado.iteracaoMelhor, resultado.alpha, duracao);
            }
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