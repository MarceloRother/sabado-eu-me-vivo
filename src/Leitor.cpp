#include "../include/Leitor.hpp"
#include <fstream>
#include <iostream>
#include <cmath>     // Para sqrt e pow
#include <algorithm> // Para lidar com strings
#include <vector>

using namespace std;

// Struct auxiliar
struct Ponto {
    double x, y;
};

// Extrai o número de vértices do NOME do arquivo (ex: "data/crd100" -> 100)
int Leitor::extrairNumeroVertices(string caminho) {
    // Remove o diretório
    string nomeBase = caminho.substr(caminho.find_last_of("/\\") + 1);

    string numeros = "";
    bool encontrouDigito = false;

    for (char c : nomeBase) {
        if (isdigit(c)) {
            numeros += c;
            encontrouDigito = true;
        } else if (encontrouDigito) {
            // Se já tínhamos achado números e agora veio uma letra, paramos.
            break; 
        }
    }

    if (numeros.empty()) return 0;
    return stoi(numeros);
}

// Lógica para arquivos de Coordenadas
Grafo* Leitor::lerArquivoCRD(string caminhoArquivo) {
    ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro fatal: Nao foi possivel abrir o arquivo " << caminhoArquivo << endl;
        exit(1);
    }

    int n = extrairNumeroVertices(caminhoArquivo);
    cout << "Lendo arquivo CRD (Coordenadas). Vertices detectados: " << n << endl;

    Grafo* grafo = new Grafo(n);
    vector<Ponto> coordenadas(n);

    // Lê as coordenadas do arquivo
    for (int i = 0; i < n; i++) {
        // Lê X e Y. Se o arquivo tiver ID antes (ex: "1 45 60"), 
        // adicione uma variavel 'dummy' antes: arquivo >> dummy >> x >> y;
        arquivo >> coordenadas[i].x >> coordenadas[i].y;
    }
    arquivo.close();

    // Calcula distâncias Euclidianas (Todos para Todos)
    // Distancia = sqrt( (x1-x2)^2 + (y1-y2)^2 )
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dx = coordenadas[i].x - coordenadas[j].x;
            double dy = coordenadas[i].y - coordenadas[j].y;
            
            double dist = sqrt(pow(dx, 2) + pow(dy, 2));
            
            // "Truncate to integer" conforme especificação do dataset
            int peso = static_cast<int>(dist);

            grafo->adicionarAresta(i, j, peso);
        }
    }

    return grafo;
}

Grafo* Leitor::lerInstancia(string caminho) {
    // 1. Isola o nome do arquivo para verificação
    // find_last_of retorna a posição da última barra. Somamos +1 para pegar o texto depois dela.
    // Se não achar barra, npos + 1 resulta em 0 (começo da string), o que funciona bem.
    size_t posBarra = caminho.find_last_of("/\\");
    string nomeArquivo = (posBarra == string::npos) ? caminho : caminho.substr(posBarra + 1);

    // 2. Verifica se "crd" está presente no nome do arquivo
    if (nomeArquivo.find("crd") != string::npos) {
        // Ex: "crd100", "data/crd20"
        return lerArquivoCRD(caminho);
    }
}