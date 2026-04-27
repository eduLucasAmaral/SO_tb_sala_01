#pragma once
#include <vector>
#include <cstddef>

// Tabela de paginas invertida simples: vetor de tamanho "frames" que mapeia frame -> pagina
// Busca de pagina -> frame é feita por varredura linear (pior caso O(N)).
class TabelaInvertida {
public:
    explicit TabelaInvertida(std::size_t frames);
    // Mapeia: coloca a pagina no frame especificado
    void mapear(int pagina, int frame);
    // Busca: retorna frame ou -1
    int buscar(int pagina) const;
    std::size_t numFrames() const;

private:
    std::vector<int> inv; // inv[frame] = pagina ou -1 se livre
};
