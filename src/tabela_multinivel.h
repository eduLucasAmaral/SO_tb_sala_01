#pragma once
#include <unordered_map>
#include <vector>
#include <cstddef>

// Tabela de paginas multinivel (2 niveis)
// Level1 é um vetor de ponteiros para tabelas de segundo nivel
// Segundas tabelas sao alocadas sob demanda para economizar memoria.
class TabelaMultinivel {
public:
    TabelaMultinivel(int tamNivel1, int tamNivel2);
    ~TabelaMultinivel();

    // Mapeia uma pagina logica para um frame fisico
    void mapear(int pagina, int frame);

    // Busca: retorna frame ou -1 se Page Fault
    int buscar(int pagina) const;

    // Uso aproximado de memoria: quantas tabelas de segundo nivel foram alocadas
    std::size_t usoMemoria() const;

private:
    int n1;
    int n2;
    std::vector<std::unordered_map<int,int>*> nivel1;
};
