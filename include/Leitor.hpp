#ifndef LEITOR_HPP
#define LEITOR_HPP

#include "Grafo.hpp"
#include <string>

class Leitor {
public:
    static Grafo* lerInstancia(std::string caminhoArquivo);

private:
    // Métodos auxiliares agora declarados aqui
    static int extrairNumeroVertices(std::string caminho);
    static Grafo* lerArquivoCRD(std::string caminhoArquivo);
};

#endif