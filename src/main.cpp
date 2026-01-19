#include <iostream>
#include <vector>
#include <list>
#include <queue>   // Para fila de prioridade (Dijkstra)
#include <limits>  // Para usar o "Infinito"
#include <fstream>
#include <chrono>
#include <ctime>   // Necessário para time()
#include <cstdlib> // Necessário para srand() e atoi()
#include <random>
#include <sstream>
#include <unordered_map>
#include <iomanip>

#include "../include/Grafo.hpp"
#include "../include/Leitor.hpp"

using namespace std;

static void salvarResultadosCSVRun(
    const string& caminhoCsv,
    const string& instancia,
    int d,
    const string& algoritmo,
    int runIdx,
    unsigned int seed,
    int melhorCusto,
    int iteracaoMelhor,
    float alpha,
    double tempoSeg
) {
    // Verifica se o arquivo existe para adicionar cabeçalho na primeira vez
    ifstream verifica(caminhoCsv);
    bool arquivoExiste = verifica.good();
    verifica.close();
    
    ofstream arquivo(caminhoCsv, ios::app);
    if (arquivo.is_open()) {
        // Adiciona cabeçalho se o arquivo não existia
        if (!arquivoExiste) {
            arquivo << "Instancia,d,Algoritmo,Run,Seed,MelhorCusto,IteracaoMelhor,Alpha,TempoSeg\n";
        }
        arquivo << instancia << "," << d << "," << algoritmo << "," << runIdx << "," << seed << ","
                << melhorCusto << "," << iteracaoMelhor << "," << fixed << setprecision(6) << alpha << ","
                << fixed << setprecision(6) << tempoSeg << "\n";
        arquivo.close();
    }
}

void salvarResultadosCSV(const string& algoritmo, unsigned int seed, int melhorCusto, int iteracaoMelhor, float alpha, long long tempoExecucaoMs) {
    // Verifica se o arquivo existe para adicionar cabeçalho na primeira vez
    ifstream verifica("resultados.csv");
    bool arquivoExiste = verifica.good();
    verifica.close();
    
    ofstream arquivo("resultados.csv", ios::app);
    if (arquivo.is_open()) {
        // Adiciona cabeçalho se o arquivo não existia
        if (!arquivoExiste) {
            arquivo << "Algoritmo,Seed,MelhorCusto,IteracaoMelhor,Alpha,TempoExecucaoMs\n";
        }
        arquivo << algoritmo << "," << seed << "," << melhorCusto << "," << iteracaoMelhor << "," << alpha << "," << tempoExecucaoMs << "\n";
        arquivo.close();
    }
}

static void salvarResultadosCSVResumo(
    const string& caminhoCsv,
    const string& instancia,
    int d,
    const string& algoritmo,
    unsigned int seed,
    int runs,
    int bestKnown,
    int melhorDas10,
    double mediaMelhor10,
    double tempoMedioSeg10
) {
    ifstream verifica(caminhoCsv);
    bool arquivoExiste = verifica.good();
    verifica.close();

    ofstream arquivo(caminhoCsv, ios::app);
    if (!arquivo.is_open()) return;

    if (!arquivoExiste) {
        arquivo << "Instancia,d,Algoritmo,Seed,Seeds,Runs,BestKnown,MelhorDas10,MediaMelhor10,TempoMedioSeg10,RPD_Melhor(%),RPD_Media(%)\n";
    }

    auto rpd = [&](double valor) -> double {
        if (bestKnown <= 0) return 0.0;
        return ((valor - (double)bestKnown) / (double)bestKnown) * 100.0;
    };

    // Seeds usadas no batch: seed0..seed0+(runs-1)
    string seedsRange = to_string(seed) + "-" + to_string(seed + (unsigned int)max(0, runs - 1));

    arquivo << instancia << "," << d << "," << algoritmo << "," << seed << "," << seedsRange << "," << runs << ","
            << bestKnown << ","
            << melhorDas10 << "," << fixed << setprecision(6) << mediaMelhor10 << ","
            << fixed << setprecision(6) << tempoMedioSeg10 << ","
            << fixed << setprecision(6) << rpd((double)melhorDas10) << ","
            << fixed << setprecision(6) << rpd(mediaMelhor10) << "\n";
    arquivo.close();
}

static unordered_map<string, int> carregarBestKnownPorInstanciaD(const string& caminho, const vector<string>& instancias, const vector<int>& ds) {
    unordered_map<string, int> best; // chave: "crd100|3"
    ifstream in(caminho);
    if (!in.is_open()) return best;

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        if (line.rfind("Data set", 0) == 0) continue;

        istringstream iss(line);
        string dataset;
        int d;
        int custo;
        if (!(iss >> dataset >> d >> custo)) continue;

        // Filtra somente instâncias que vamos rodar (crd*)
        // e somente ds de interesse.
        bool okInst = false;
        for (auto &x : instancias) if (x == dataset) { okInst = true; break; }
        if (!okInst) continue;

        bool okD = false;
        for (auto &x : ds) if (x == d) { okD = true; break; }
        if (!okD) continue;

        best[dataset + "|" + to_string(d)] = custo;
    }
    return best;
}

static vector<float> parseFloatList(const string& s) {
    vector<float> out;
    string item;
    stringstream ss(s);
    while (getline(ss, item, ',')) {
        if (item.empty()) continue;
        out.push_back((float)atof(item.c_str()));
    }
    return out;
}

static vector<int> parseIntList(const string& s) {
    vector<int> out;
    string item;
    stringstream ss(s);
    while (getline(ss, item, ',')) {
        if (item.empty()) continue;
        out.push_back(atoi(item.c_str()));
    }
    return out;
}

static vector<string> listarInstanciasCrd(const string& pastaData) {
    // Lista hardcoded das instâncias crd disponíveis
    vector<string> inst;
    // crd100 a crd109
    for (int i = 0; i <= 9; i++) {
        inst.push_back("crd10" + to_string(i));
    }
    // crd300 a crd309
    for (int i = 0; i <= 9; i++) {
        inst.push_back("crd30" + to_string(i));
    }
    // crd500 a crd509
    for (int i = 0; i <= 9; i++) {
        inst.push_back("crd50" + to_string(i));
    }
    // crd700 a crd709
    for (int i = 0; i <= 9; i++) {
        inst.push_back("crd70" + to_string(i));
    }
    return inst;
}

int main(int argc, char* argv[]) {
    // Se rodar em modo batch:
    //   bin/meu_programa.exe --batch
    // opções (todas opcionais):
    //   --alphas 0.1,0.3,0.5
    //   --d 2,3,4,5
    //   --runs 10
    //   --grasp-iters 30
    //   --reativo-iters 300
    //   --reativo-block 30
    //   --seed0 12345   (seed base; cada run usa seed0 + runIdx)
    bool modoBatch = false;
    string alphasStr = "0.1,0.3,0.5";
    string dsStr = "2,3,4,5";
    int runs = 10;
    int graspIters = 30;
    int reativoIters = 300;
    int reativoBlock = 30;
    unsigned int seed = 0; // será definida depois se necessário

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--batch") modoBatch = true;
        else if (arg == "--alphas" && i + 1 < argc) alphasStr = argv[++i];
        else if (arg == "--d" && i + 1 < argc) dsStr = argv[++i];
        else if (arg == "--runs" && i + 1 < argc) runs = atoi(argv[++i]);
        else if (arg == "--grasp-iters" && i + 1 < argc) graspIters = atoi(argv[++i]);
        else if (arg == "--reativo-iters" && i + 1 < argc) reativoIters = atoi(argv[++i]);
        else if (arg == "--reativo-block" && i + 1 < argc) reativoBlock = atoi(argv[++i]);
        else if (arg == "--seed" && i + 1 < argc) seed = (unsigned int)atoi(argv[++i]);
    }

    if (modoBatch) {
        // Se seed não foi passada via --seed, usa time(0) para variar entre execuções
        if (seed == 0) {
            seed = (unsigned int)time(0);
        }
        // Caminhos relativos ao diretório raiz do projeto
        const string pastaData = "data";
        const string csvRuns = "resultados_runs.csv";
        const string csvResumo = "resultados_resumo.csv";

        vector<string> instancias = listarInstanciasCrd(pastaData);
        vector<float> alphas = parseFloatList(alphasStr);
        vector<int> ds = parseIntList(dsStr);

        auto bestKnownMap = carregarBestKnownPorInstanciaD("data/bestSolutions.txt", instancias, ds);

        for (auto &inst : instancias) {
            string caminhoInst = pastaData + "/" + inst;

            for (int d : ds) {
                // Carrega grafo para esta instância/d
                Grafo* g = Leitor::lerInstancia(caminhoInst);
                g->setRestricaoConexoes(d);

                int bestKnown = 0;
                auto itBK = bestKnownMap.find(inst + "|" + to_string(d));
                if (itBK != bestKnownMap.end()) bestKnown = itBK->second;

                // ---------- GA (determinístico; ainda assim repetimos 10x pra tempo médio) ----------
                {
                    int melhorDas10 = 0;
                    double somaMelhor = 0.0;
                    double somaTempo = 0.0;

                    for (int run = 0; run < runs; run++) {
                        // Guloso não usa seed (determinístico), mas registramos 0 para consistência
                        auto ini = chrono::high_resolution_clock::now();
                        ResultadoAlgoritmo r = g->GA(d);
                        auto fim = chrono::high_resolution_clock::now();
                        double tempoSeg = chrono::duration_cast<chrono::duration<double>>(fim - ini).count();

                        if (run == 0 || r.melhorCusto < melhorDas10) melhorDas10 = r.melhorCusto;
                        somaMelhor += (double)r.melhorCusto;
                        somaTempo += tempoSeg;

                        salvarResultadosCSVRun(csvRuns, inst, d, "guloso", run, 0, r.melhorCusto, r.iteracaoMelhor, r.alpha, tempoSeg);
                    }

                    salvarResultadosCSVResumo(csvResumo, inst, d, "guloso", 0, runs, bestKnown, melhorDas10, somaMelhor / runs, somaTempo / runs);
                }

                // ---------- GRASP (randomizado): 3 alfas * 10 seeds; construtivo >= 30 ----------
                for (float alpha : alphas) {
                    int melhorDas10 = 0;
                    double somaMelhor = 0.0;
                    double somaTempo = 0.0;

                    for (int run = 0; run < runs; run++) {
                        unsigned int runSeed = seed + (unsigned int)run;
                        srand(runSeed);
                        auto ini = chrono::high_resolution_clock::now();
                        ResultadoAlgoritmo r = g->GRASP(alpha, graspIters, d);
                        auto fim = chrono::high_resolution_clock::now();
                        double tempoSeg = chrono::duration_cast<chrono::duration<double>>(fim - ini).count();

                        if (run == 0 || r.melhorCusto < melhorDas10) melhorDas10 = r.melhorCusto;
                        somaMelhor += (double)r.melhorCusto;
                        somaTempo += tempoSeg;

                        string nomeAlg = string("grasp_alpha=") + to_string(alpha);
                        salvarResultadosCSVRun(csvRuns, inst, d, nomeAlg, run, runSeed, r.melhorCusto, r.iteracaoMelhor, r.alpha, tempoSeg);
                    }

                    string nomeAlgResumo = string("grasp_alpha=") + to_string(alpha);
                    salvarResultadosCSVResumo(csvResumo, inst, d, nomeAlgResumo, seed, runs, bestKnown, melhorDas10, somaMelhor / runs, somaTempo / runs);
                }

                // ---------- GRASP Reativo: construtivo >= 300; blocos 30..50 ----------
                {
                    int melhorDas10 = 0;
                    double somaMelhor = 0.0;
                    double somaTempo = 0.0;

                    for (int run = 0; run < runs; run++) {
                        unsigned int runSeed = seed + (unsigned int)run;
                        srand(runSeed);
                        auto ini = chrono::high_resolution_clock::now();
                        ResultadoAlgoritmo r = g->GRASPReativo(alphas, reativoIters, reativoBlock, d, (int)runSeed);
                        auto fim = chrono::high_resolution_clock::now();
                        double tempoSeg = chrono::duration_cast<chrono::duration<double>>(fim - ini).count();

                        if (run == 0 || r.melhorCusto < melhorDas10) melhorDas10 = r.melhorCusto;
                        somaMelhor += (double)r.melhorCusto;
                        somaTempo += tempoSeg;

                        salvarResultadosCSVRun(csvRuns, inst, d, "grasp_reativo", run, runSeed, r.melhorCusto, r.iteracaoMelhor, r.alpha, tempoSeg);
                    }

                    salvarResultadosCSVResumo(csvResumo, inst, d, "grasp_reativo", seed, runs, bestKnown, melhorDas10, somaMelhor / runs, somaTempo / runs);
                }

                delete g;
            }
        }

        cout << "Batch finalizado. Arquivos gerados:\n";
        cout << " - resultados_runs.csv\n";
        cout << " - resultados_resumo.csv\n";
        return 0;
    }

    // Caminho padrão para o modo interativo (diretório raiz do projeto)
    string arquivo = "data/crd100";

    seed = (unsigned int)time(0);
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
                salvarResultadosCSV("guloso", seed, resultado.melhorCusto, resultado.iteracaoMelhor, resultado.alpha, duracao);
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
                salvarResultadosCSV("grasp", seed, resultado.melhorCusto, resultado.iteracaoMelhor, resultado.alpha, duracao);
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
                salvarResultadosCSV("grasp_reativo", seed, resultado.melhorCusto, resultado.iteracaoMelhor, resultado.alpha, duracao);
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