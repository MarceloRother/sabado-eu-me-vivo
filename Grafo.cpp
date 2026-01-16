#include "Grafo.hpp"

#include <iostream>
#include <vector>
#include <list>
#include <queue>      // Para fila de prioridade (Dijkstra)
#include <limits>     // Para usar o "Infinito"
#include <algorithm>  // Para sort em Kruskal
#include <functional> // Para std::function
#include <stack>      // Para busca em profundidade

using namespace std;

// Adicionar aresta direcionada (Origem -> Destino)
void Grafo::adicionarAresta(int origem, int destino, int peso){
    // Insere o peso na matriz de adjacência
    adj[origem][destino] = peso;
    if(tipo == NAO_DIRECIONADO){
        // Se for não direcionado, insere a aresta de volta
        adj[destino][origem] = peso;
    }
}

/*void Grafo::dijkstra(int inicio)
{
    // Vetor de distâncias iniciada com "Infinito"
    vector<int> dist(numeroDeVertices, numeric_limits<int>::max());

    // Fila de prioridade para pegar sempre o menor caminho atual
    // (peso, vertice_atual) -> C++ ordena pelo primeiro elemento do par
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[inicio] = 0;
    pq.push({0, inicio});

    while (!pq.empty())
    {
        int distanciaAtual = pq.top().first;
        int u = pq.top().second; // Vértice atual
        pq.pop();

        // Se achamos um caminho maior que o já conhecido, ignora
        if (distanciaAtual > dist[u])
            continue;

        // Explora os vizinhos (percorre a lista de arestas do nó u)
        for (const auto &aresta : adj[u]){
            int v = aresta.destino;
            int pesoAresta = aresta.peso;

            // Relaxamento (Se achei um caminho melhor...)
            if (dist[u] + pesoAresta < dist[v])
            {
                dist[v] = dist[u] + pesoAresta;
                pq.push({dist[v], v});
            }
        }
    }

    // Imprimir resultados
    cout << "Distancias a partir do no " << inicio << ":\n";
    for (int i = 0; i < numeroDeVertices; i++)
    {
        cout << "No " << i << ": " << dist[i] << endl;
    }
}*/

/*void Grafo::floyd(){
    // Matriz de distâncias
    vector<vector<int>> dist(numeroDeVertices, vector<int>(numeroDeVertices, numeric_limits<int>::max()));

    // Inicializa a matriz com as arestas do grafo
    for (int u = 0; u < numeroDeVertices; u++){
        dist[u][u] = 0; // Distância para si mesmo é zero
        for (const auto &aresta : adj[u]){
            int v = aresta.destino;
            int peso = aresta.peso;
            dist[u][v] = peso; // Distância direta pela aresta
        }
    }

    // Algoritmo de Floyd-Warshall
    for (int k = 0; k < numeroDeVertices; k++)
    { // Nó intermediário
        for (int i = 0; i < numeroDeVertices; i++)
        { // Nó de origem
            for (int j = 0; j < numeroDeVertices; j++)
            { // No de destino
                if (dist[i][k] != numeric_limits<int>::max() && dist[k][j] != numeric_limits<int>::max())
                {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Imprimir a matriz de distâncias
    cout << "Matriz de distancias (Floyd-Warshall):\n";
    for (int i = 0; i < numeroDeVertices; i++)
    {
        for (int j = 0; j < numeroDeVertices; j++)
        {
            if (dist[i][j] == numeric_limits<int>::max())
            {
                cout << "INF ";
            }
            else
            {
                cout << dist[i][j] << " ";
            }
        }
        cout << endl;
    }
}*/

/*void Grafo::buscaEmProfundidade(int inicio){
    if (inicio < 0 || inicio > numeroDeVertices - 1) return;

    // Inicializa vetor de visitados
    vector<bool> visited(numeroDeVertices, false);

    // Stack de busca
    stack<int> neighbours;

    // Start visitado
    visited[inicio] = true;
    neighbours.push(inicio);

    cout << "DFS a partir do vertice " << inicio << ": ";

    while(!neighbours.empty()) {
    int v = neighbours.top();
    neighbours.pop();

    cout << v << " ";

    for (const auto& edge : adj[v]) {
        int neighbour = edge.destino;
            if (!visited[neighbour]) {
                visited[neighbour] = true;
                neighbours.push(neighbour);
            }
        }
    }
    cout << endl;
}*/

// Prim
// Algoritmo para encontrar a Árvore Geradora Mínima (AGM)
// Guloso
void Grafo::prim(int r){
    // Cria uma estrutura para as arestas
    struct Aresta {
        int origem;
        int destino;
        int peso;
        
        Aresta(int u, int v, int p) : origem(u), destino(v), peso(p) {}
    };

    // Lógica INVERTIDA para Min-Heap
    // Usada para a PQ
    struct ComparaPeso {
        bool operator()(const Aresta& a, const Aresta& b) {
            return a.peso > b.peso;
        }
    };

    vector<Aresta> resultado;
    int custoTotal = 0;

    // 1. Min-Heap para pegar sempre a aresta mais leve automaticamente
    // <Aresta, vector<Aresta>, Comparador>
    priority_queue<Aresta, vector<Aresta>, ComparaPeso> pq;

    // 2. Vetor para marcar nós visitados
    vector<bool> visitado(numeroDeVertices, false);
    
    // Adiciona as arestas iniciais
    visitado[r] = true;
    for(int i = 0; i < numeroDeVertices; i++){
        if(adj[r][i] != 0){ // Se existe aresta
            pq.push(Aresta(r, i, adj[r][i]));
        }
    }

    cout << "Iniciando Prim a partir do no " << r << "...\n";

    while(!pq.empty()){
        // Pega a aresta mais leve disponível (de QUALQUER nó visitado anteriormente)
        Aresta atual = pq.top();
        pq.pop();

        int destino = atual.destino;

        // SE O DESTINO JÁ FOI VISITADO, IGNORA (Evita ciclos)
        if(visitado[destino]) {
            continue;
        }

        // Se não foi visitado, aceitamos a aresta na AGM
        visitado[destino] = true;
        resultado.push_back(atual);
        custoTotal += atual.peso;

        // Adiciona as arestas do NOVO nó descoberto na fila
        // Elas vão competir com as arestas antigas que ainda estão lá
        for(int i = 0; i < numeroDeVertices; i++){
            if(adj[destino][i] != 0 && !visitado[i]){
                pq.push(Aresta(destino, i, adj[destino][i]));
            }
        }
    }

    // Impressão
    cout << "Arestas da AGM (Prim):\n";
    for(const auto& aresta : resultado){
        cout << aresta.origem << " -- " << aresta.destino 
             << " (Peso: " << aresta.peso << ")\n";
    }
    cout << "Custo Total: " << custoTotal << endl;
}

// Algoritmo Guloso Randomizado (baseado em Prim)
// Não segue a lógica tradicional de um algoritmo guloso
// A cada iteração ele tem k opções (as k menores arestas disponíveis) e escolhe uma aleatoriamente
void Grafo::random(int r, int k){
    // Implementação alternativa de Prim (se necessário)
    // Cria uma estrutura para as arestas
    struct Aresta {
        int origem;
        int destino;
        int peso;
        
        Aresta(int u, int v, int p) : origem(u), destino(v), peso(p) {}
    };

    // Lógica INVERTIDA para Min-Heap
    // Usada para a PQ
    struct ComparaPeso {
        bool operator()(const Aresta& a, const Aresta& b) {
            return a.peso > b.peso;
        }
    };

    vector<Aresta> resultado;
    int custoTotal = 0;

    // 1. Min-Heap para pegar sempre a aresta mais leve automaticamente
    // <Aresta, vector<Aresta>, Comparador>
    priority_queue<Aresta, vector<Aresta>, ComparaPeso> pq;

    // 2. Vetor para marcar nós visitados
    vector<bool> visitado(numeroDeVertices, false);
    
    // Adiciona as arestas iniciais
    visitado[r] = true;
    for(int i = 0; i < numeroDeVertices; i++){
        if(adj[r][i] != 0){ // Se existe aresta
            pq.push(Aresta(r, i, adj[r][i]));
        }
    }

    while(!pq.empty()){
        
        // 1. Criar lista temporária de candidatos para ESTA rodada
        vector<Aresta> candidatos;
        
        // Tenta pegar até 'k' arestas, mas ignora as inválidas (já visitadas)
        // Isso evita encher a lista de lixo
        int contagem = 0;
        while(!pq.empty() && contagem < k) {
            Aresta a = pq.top();
            pq.pop();
            
            if(!visitado[a.destino]) {
                candidatos.push_back(a);
                contagem++;
            }
        }

        // Se não achou nenhum candidato válido, acabou (ou grafo desconexo)
        if (candidatos.empty()) break;

        // 2. Sorteia um índice entre os candidatos válidos coletados
        int escolha = rand() % candidatos.size();
        Aresta arestaEscolhida = candidatos[escolha];

        // 3. IMPORTANTE: Devolve os NÃO escolhidos para a fila principal
        // Eles ainda são boas opções para o futuro!
        for(int i = 0; i < candidatos.size(); i++) {
            if (i != escolha) {
                pq.push(candidatos[i]);
            }
        }

        // 4. Processa a aresta vencedora
        int u = arestaEscolhida.origem; // Se precisar
        int v = arestaEscolhida.destino;

        // O 'if visitado' aqui é redundante pois filtramos antes, 
        // mas mal não faz (seguro morreu de velho)
        if(visitado[v]) continue; 

        visitado[v] = true;
        resultado.push_back(arestaEscolhida);
        custoTotal += arestaEscolhida.peso;

        // 5. Adiciona os vizinhos do novo nó
        for(int i = 0; i < numeroDeVertices; i++){
            // Assumindo matriz de adjacência (peso 0 = sem aresta)
            if(adj[v][i] != 0 && !visitado[i]){
                pq.push(Aresta(v, i, adj[v][i]));
            }
        }
    }
    // Impressão
    cout << "Arestas do Algoritmo Guloso Randomizado:\n";
    for(const auto& aresta : resultado){
        cout << aresta.origem << " -- " << aresta.destino 
             << " (Peso: " << aresta.peso << ")\n";
    }
    cout << "Custo Total: " << custoTotal << endl;
}