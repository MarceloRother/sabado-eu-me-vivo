#include "Grafo.hpp"

#include <iostream>
#include <vector>
#include <list>
#include <queue>      // Para fila de prioridade
#include <limits>     // Para usar o "Infinito"
#include <functional> // Para std::function
#include <algorithm>

using namespace std;

// Cria uma estrutura para as arestas
struct Aresta
{
    int origem;
    int destino;
    int peso;

    Aresta(int u, int v, int p) : origem(u), destino(v), peso(p) {}
};

// Lógica INVERTIDA para Min-Heap
// Usada para a PQ
struct ComparaPeso
{
    bool operator()(const Aresta &a, const Aresta &b)
    {
        return a.peso > b.peso;
    }
};

// Adicionar aresta direcionada (Origem -> Destino)
void Grafo::adicionarAresta(int origem, int destino, int peso)
{
    // Insere o peso na matriz de adjacência
    adj[origem][destino] = peso;
    if (tipo == NAO_DIRECIONADO)
    {
        // Se for não direcionado, insere a aresta de volta
        adj[destino][origem] = peso;
    }
}

// IMPLEMENTAÇÕES DOS ALGORITMOS

// GA para encontrar a Árvore Geradora Mínima (AGM) baseado em Prim
void Grafo::GA(int r)
{
    vector<Aresta> resultado;
    int custoTotal = 0;

    // 1. Min-Heap para pegar sempre a aresta mais leve automaticamente
    // <Aresta, vector<Aresta>, Comparador>
    priority_queue<Aresta, vector<Aresta>, ComparaPeso> pq;

    // 2. Vetor para marcar nós visitados
    vector<bool> visitado(numeroDeVertices, false);

    // Adiciona as arestas iniciais
    visitado[r] = true;
    for (int i = 0; i < numeroDeVertices; i++)
    {
        if (adj[r][i] != 0)
        { // Se existe aresta
            pq.push(Aresta(r, i, adj[r][i]));
        }
    }

    cout << "Iniciando Prim a partir do no " << r << "...\n";

    while (!pq.empty())
    {
        // Pega a aresta mais leve disponível (de QUALQUER nó visitado anteriormente)
        Aresta atual = pq.top();
        pq.pop();

        int destino = atual.destino;

        // SE O DESTINO JÁ FOI VISITADO, IGNORA (Evita ciclos)
        if (visitado[destino])
        {
            continue;
        }

        // Se não foi visitado, aceitamos a aresta na AGM
        visitado[destino] = true;
        resultado.push_back(atual);
        custoTotal += atual.peso;

        // Adiciona as arestas do NOVO nó descoberto na fila
        // Elas vão competir com as arestas antigas que ainda estão lá
        for (int i = 0; i < numeroDeVertices; i++)
        {
            if (adj[destino][i] != 0 && !visitado[i])
            {
                pq.push(Aresta(destino, i, adj[destino][i]));
            }
        }
    }

    // Impressão
    cout << "Arestas da AGM (Prim):\n";
    for (const auto &aresta : resultado)
    {
        cout << aresta.origem << " -- " << aresta.destino
             << " (Peso: " << aresta.peso << ")\n";
    }
    cout << "Custo Total: " << custoTotal << endl;
}

// Algoritmo Guloso Randomizado (baseado em Prim)
// Não segue a lógica tradicional de um algoritmo guloso
// Ele cria uma lista restrita de candidatos (LRC) e escolhe aleatoriamente entre eles
// LRC baseada em um alpha fixo que define o quão "restrita" ela é
void Grafo::GRASP(int r, int alpha, int interacoes)
{
    cout << "Algoritmo Guloso Randomizado:\n";
    int custoFinal = 0;

    for (int iter = 0; iter < interacoes; iter++)
    {
        vector<Aresta> resultado;
        int custoTotal = 0;

        // Min-Heap para pegar sempre a aresta mais leve automaticamente
        priority_queue<Aresta, vector<Aresta>, ComparaPeso> pq;

        // Vetor para marcar nós visitados
        vector<bool> visitado(numeroDeVertices, false);

        // Adiciona as arestas iniciais
        visitado[r] = true;
        for (int i = 0; i < numeroDeVertices; i++)
        {
            if (adj[r][i] != 0)
            { // Se existe aresta
                pq.push(Aresta(r, i, adj[r][i]));
            }
        }
        while (!pq.empty())
        {
            // Criar lista temporária de candidatos para ESTA rodada
            vector<Aresta> candidatos;

            // Tenta pegar até 'k' arestas, mas ignora as inválidas
            while (!pq.empty())
            {
                Aresta a = pq.top();
                pq.pop();
                if (!visitado[a.destino])
                {
                    candidatos.push_back(a);
                }
            }

            // Se não achou nenhum candidato válido, acabou (ou grafo desconexo)
            if (candidatos.empty())
                break;

            // Identifica c_min e c_max
            float c_min = candidatos.front().peso; // Como saiu da PQ, o 1º é o menor
            float c_max = candidatos.back().peso;  // O último é o maior

            // Calcula o Limite (Threshold)
            float limite = c_min + alpha * (c_max - c_min);

            // 4. Cria a LRC (Lista Restrita de Candidatos) filtrando pelo limite
            vector<Aresta> LRC;
            for (auto &a : candidatos)
            {
                if (a.peso <= limite)
                {
                    LRC.push_back(a);
                }
                else
                {
                    // Como está ordenado, se passou do limite, os próximos também passarão
                    // Mas PRECISAMOS salvar para devolver pra PQ depois
                    break;
                }
            }

            // Sorteia um índice entre os candidatos válidos coletados
            int escolha = rand() % LRC.size();
            Aresta arestaEscolhida = LRC[escolha];

            // Devolve os NÃO escolhidos para a fila principal
            for (auto &a : candidatos)
            {
                // Cuidado para não devolver a aresta escolhida nem arestas para nós já visitados
                if (a.destino != arestaEscolhida.destino && !visitado[a.destino])
                {
                    pq.push(a);
                }
            }

            // Processa a aresta vencedora
            int u = arestaEscolhida.origem; // Se precisar
            int v = arestaEscolhida.destino;

            // Altera o valor de visitado
            visitado[v] = true;
            resultado.push_back(arestaEscolhida);
            custoTotal += arestaEscolhida.peso;

            // Adiciona os vizinhos do novo nó
            for (int i = 0; i < numeroDeVertices; i++)
            {
                // Assumindo matriz de adjacência (peso 0 = sem aresta)
                if (adj[v][i] != 0 && !visitado[i])
                {
                    pq.push(Aresta(v, i, adj[v][i]));
                }
            }
        }
        // Impressão
        cout << "Iteração " << iter + 1 << ":\n";
        for (const auto &aresta : resultado)
        {
            cout << aresta.origem << " -- " << aresta.destino
                 << " (Peso: " << aresta.peso << ")\n";
        }
        cout << "Custo Total: " << custoTotal << endl;

        if (iter == 0 || custoTotal < custoFinal)
        {
            custoFinal = custoTotal;
        }
    }

    cout << "Melhor custo: " << custoFinal << endl;
}

// Implementação similar ao GRASP, mas com alpha variável baseado na escolha do usuário
// Aqui você pode definir diferentes valores de alpha para cada nível de aleatoriedade
void Grafo::GRASPReativo(int escolha, int interacoes, int block)
{
    // Definimos qual será a lista de alphas utilizada baseado na escolha do usuário
    vector<float> alphas;
    vector<float> probabilidades(5, 0.2); // Inicialmente iguais
    switch (escolha)
    {
    case 1:
        alphas = {0.1, 0.2, 0.3, 0.4, 0.5};
        break; // Baixo
    case 2:
        alphas = {0.1, 0.3, 0.5, 0.7, 0.9};
        break; // Médio
    case 3:
        alphas = {0.1, 0.4, 0.6, 0.8, 1.0};
        break; // Alto
    default:
        break;
    }

    cout << "Algoritmo Guloso Randomizado Reativo:\n";
    int custoFinal = 0;
    vector<vector<int>> desempenho(5, vector<int>(interacoes, 0)); // Para armazenar desempenho por alpha

    for (int iter = 0; iter < interacoes; iter++)
    {
        // Aqui é feita a atualização das probabilidades baseada no desempenho do bloco anterior
        if(iter % block == 0 && iter != 0){
            // Atualiza probabilidades baseado em desempenho
            vector<pair<int, int>> somaDesempenho(5, {0, 0}); // {indice, soma}
            for(int i = 0; i < 5; i++){
                for(int j = 0; j < desempenho[i].size(); j++){
                    somaDesempenho[i].first = i;
                    somaDesempenho[i].second += desempenho[i][j];
                }
            }
            // Ordena pelo desempenho (menor é melhor)
            sort(somaDesempenho.begin(), somaDesempenho.end(), [](auto &left, auto &right) {
                return left.second < right.second;
            });

            // 1° Melhor desempenho (+0.1)
            if(probabilidades[somaDesempenho[0].first] <= 0.9){
                probabilidades[somaDesempenho[0].first] += 0.1;
            }

            // 2° Melhor desemepenho (+0.1)
            if(probabilidades[somaDesempenho[1].first] <= 0.9){
                probabilidades[somaDesempenho[1].first] += 0.1;
            }

            // 3° Melhor desemepenho (nada)

            // 4° Pior desempenho (-0.1)
            if(probabilidades[somaDesempenho[3].first] <= 0.9){
                probabilidades[somaDesempenho[3].first] -= 0.1;
            }

            // 5° Pior desempenho (-0.1)
            if(probabilidades[somaDesempenho[4].first] <= 0.9){
                probabilidades[somaDesempenho[4].first] -= 0.1;
            }

        }
        vector<Aresta> resultado;
            int custoTotal = 0;

            // Min-Heap para pegar sempre a aresta mais leve automaticamente
            priority_queue<Aresta, vector<Aresta>, ComparaPeso> pq;

            // Vetor para marcar nós visitados
            vector<bool> visitado(numeroDeVertices, false);

            // Adiciona as arestas iniciais
            visitado[r] = true;
            for (int i = 0; i < numeroDeVertices; i++)
            {
                if (adj[r][i] != 0)
                { // Se existe aresta
                    pq.push(Aresta(r, i, adj[r][i]));
                }
            }
            while (!pq.empty())
            {
                // Criar lista temporária de candidatos para ESTA rodada
                vector<Aresta> candidatos;

                // Tenta pegar até 'k' arestas, mas ignora as inválidas
                while (!pq.empty())
                {
                    Aresta a = pq.top();
                    pq.pop();
                    if (!visitado[a.destino])
                    {
                        candidatos.push_back(a);
                    }
                }

                // Se não achou nenhum candidato válido, acabou (ou grafo desconexo)
                if (candidatos.empty())
                    break;

                // Identifica c_min e c_max
                float c_min = candidatos.front().peso; // Como saiu da PQ, o 1º é o menor
                float c_max = candidatos.back().peso;  // O último é o maior

                // Calcula o Limite (Threshold)
                float limite = c_min + alpha * (c_max - c_min);

                // 4. Cria a LRC (Lista Restrita de Candidatos) filtrando pelo limite
                vector<Aresta> LRC;
                for (auto &a : candidatos)
                {
                    if (a.peso <= limite)
                    {
                        LRC.push_back(a);
                    }
                    else
                    {
                        // Como está ordenado, se passou do limite, os próximos também passarão
                        // Mas PRECISAMOS salvar para devolver pra PQ depois
                        break;
                    }
                }

                // Sorteia um índice entre os candidatos válidos coletados
                int escolha = rand() % LRC.size();
                Aresta arestaEscolhida = LRC[escolha];

                // Devolve os NÃO escolhidos para a fila principal
                for (auto &a : candidatos)
                {
                    // Cuidado para não devolver a aresta escolhida nem arestas para nós já visitados
                    if (a.destino != arestaEscolhida.destino && !visitado[a.destino])
                    {
                        pq.push(a);
                    }
                }

                // Processa a aresta vencedora
                int u = arestaEscolhida.origem; // Se precisar
                int v = arestaEscolhida.destino;

                // Altera o valor de visitado
                visitado[v] = true;
                resultado.push_back(arestaEscolhida);
                custoTotal += arestaEscolhida.peso;

                // Adiciona os vizinhos do novo nó
                for (int i = 0; i < numeroDeVertices; i++)
                {
                    // Assumindo matriz de adjacência (peso 0 = sem aresta)
                    if (adj[v][i] != 0 && !visitado[i])
                    {
                        pq.push(Aresta(v, i, adj[v][i]));
                    }
                }
            }
            // Impressão
            cout << "Iteração " << iter + 1 << ":\n";
            for (const auto &aresta : resultado)
            {
                cout << aresta.origem << " -- " << aresta.destino
                     << " (Peso: " << aresta.peso << ")\n";
            }
            cout << "Custo Total: " << custoTotal << endl;

            if (iter == 0 || custoTotal < custoFinal)
            {
                custoFinal = custoTotal;
            }
    }

    cout << "Melhor custo: " << custoFinal << endl;
}