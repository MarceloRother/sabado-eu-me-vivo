@echo off
echo ========================================
echo Executando experimentos em modo batch
echo ========================================
echo.
echo Parametros padrao:
echo   - Instancias: crd100-crd109, crd300-crd309, crd500-crd509, crd700-crd709 (40 instancias)
echo   - Valores de d: 2, 3, 4, 5
echo   - Alphas GRASP: 0.1, 0.3, 0.5
echo   - Runs por configuracao: 10
echo   - Iteracoes GRASP: 30
echo   - Iteracoes GRASP Reativo: 300
echo   - Bloco GRASP Reativo: 30
echo.
echo Total estimado: ~10800 execucoes
echo.
pause

bin\meu_programa.exe --batch --alphas 0.1,0.3,0.5 --d 2,3,4,5 --runs 10 --grasp-iters 30 --reativo-iters 300 --reativo-block 30 --seed0 12345

echo.
echo ========================================
echo Experimentos finalizados!
echo ========================================
echo.
echo Arquivos gerados:
echo   - resultados_runs.csv (todas as execucoes individuais)
echo   - resultados_resumo.csv (resumo com medias e desvios percentuais)
echo.
pause
