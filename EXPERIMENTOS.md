# Guia de Execução dos Experimentos

## Modo Batch

O programa suporta execução em modo batch para rodar todos os experimentos automaticamente.

### Execução Rápida

Execute o arquivo `executar_experimentos.bat` ou use:

```bash
bin\meu_programa.exe --batch
```

### Parâmetros Opcionais

Você pode customizar os parâmetros:

```bash
bin\meu_programa.exe --batch \
  --alphas 0.1,0.3,0.5 \
  --d 2,3,4,5 \
  --runs 10 \
  --grasp-iters 30 \
  --reativo-iters 300 \
  --reativo-block 30 \
  --seed0 12345
```

**Parâmetros:**
- `--batch`: Ativa o modo batch
- `--alphas`: Valores de alpha para GRASP (separados por vírgula)
- `--d`: Valores de grau máximo (separados por vírgula)
- `--runs`: Número de execuções por configuração (padrão: 10)
- `--grasp-iters`: Iterações do algoritmo GRASP (padrão: 30)
- `--reativo-iters`: Iterações do GRASP Reativo (padrão: 300)
- `--reativo-block`: Tamanho do bloco para atualização de probabilidades (padrão: 30)
- `--seed0`: Seed base para geração de números aleatórios (padrão: 12345)

### Instâncias Testadas

O modo batch testa automaticamente todas as instâncias `crd` disponíveis:
- **crd100** a **crd109** (10 instâncias)
- **crd300** a **crd309** (10 instâncias)
- **crd500** a **crd509** (10 instâncias)
- **crd700** a **crd709** (10 instâncias)

**Total: 40 instâncias**

### Algoritmos Executados

Para cada combinação (instância, d, algoritmo):
1. **Algoritmo Guloso (GA)**: Executado 10 vezes (para tempo médio)
2. **GRASP**: Executado com 3 valores de alpha, cada um 10 vezes
3. **GRASP Reativo**: Executado 10 vezes (usa múltiplos alphas internamente)

### Arquivos Gerados

#### 1. `resultados_runs.csv`

Contém **todas as execuções individuais** com as seguintes colunas:
- `Instancia`: Nome da instância
- `d`: Valor do grau máximo
- `Algoritmo`: Nome do algoritmo (ex: "guloso", "grasp_alpha=0.1", "grasp_reativo")
- `Run`: Índice da execução (0-9)
- `Seed`: Seed usada nesta execução
- `MelhorCusto`: Melhor custo encontrado
- `IteracaoMelhor`: Iteração onde foi encontrado o melhor custo
- `Alpha`: Valor de alpha usado
- `TempoSeg`: Tempo de execução em segundos

#### 2. `resultados_resumo.csv`

Contém o **resumo estatístico** com as seguintes colunas:
- `Instancia`: Nome da instância
- `d`: Valor do grau máximo
- `Algoritmo`: Nome do algoritmo
- `BestKnown`: Melhor solução conhecida (do arquivo `bestSolutions.txt`)
- `MelhorDas10`: Melhor solução encontrada entre as 10 execuções
- `MediaMelhor10`: Média das melhores soluções das 10 execuções
- `TempoMedioSeg10`: Tempo médio das 10 execuções (em segundos)
- `RPD_Melhor(%)`: Desvio percentual relativo da melhor solução
- `RPD_Media(%)`: Desvio percentual relativo da média

**Fórmula RPD:**
```
RPD = ((valor - bestKnown) / bestKnown) * 100
```

### Exemplo de Uso

```bash
# Executar com parâmetros padrão
bin\meu_programa.exe --batch

# Executar com alphas customizados
bin\meu_programa.exe --batch --alphas 0.2,0.4,0.6

# Executar apenas com d=3 e d=4
bin\meu_programa.exe --batch --d 3,4

# Executar com mais iterações
bin\meu_programa.exe --batch --grasp-iters 50 --reativo-iters 500
```

### Estimativa de Tempo

Com os parâmetros padrão:
- **40 instâncias** × **4 valores de d** × **3 algoritmos** (GA + 3 GRASP + GRASP Reativo) × **10 runs** = **~6400 execuções**

Tempo estimado depende do hardware, mas pode levar várias horas para completar.

### Modo Interativo

Se executar sem `--batch`, o programa entra no modo interativo onde você pode escolher manualmente qual algoritmo executar.
