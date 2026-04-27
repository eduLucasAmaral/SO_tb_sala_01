#pragma once
#include <vector>
#include <utility>
#include <cstddef>

// Tabela de paginas baseada em hash com chaining
class TabelaHash {
public:
    explicit TabelaHash(std::size_t buckets);
    void mapear(int pagina, int frame);
    int buscar(int pagina) const; // retorna frame ou -1
    std::size_t numBuckets() const;
    std::size_t numEntradas() const;

private:
    std::size_t bcount;
    std::vector<std::vector<std::pair<int,int>>> tabela; // balde -> lista (pagina,frame)
};
