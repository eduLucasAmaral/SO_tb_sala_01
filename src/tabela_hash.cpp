#include "tabela_hash.h"
#include <functional>

TabelaHash::TabelaHash(std::size_t buckets)
    : bcount(buckets), tabela(buckets) {}

void TabelaHash::mapear(int pagina, int frame) {
    // Calcular indice do balde usando o hash padrao de int
    std::hash<int> h;
    std::size_t idx = h(pagina) % bcount;
    // Percorrer o balde: se a pagina ja esta mapeada, atualizar o frame
    for (auto &p : tabela[idx]) {
        if (p.first == pagina) { p.second = frame; return; }
    }
    // Caso contrario, inserir no final da lista do balde (chaining)
    tabela[idx].push_back({pagina, frame});
}

int TabelaHash::buscar(int pagina) const {
    // Calcular balde e procurar na lista encadeada (vetor) do balde
    std::hash<int> h;
    std::size_t idx = h(pagina) % bcount;
    for (const auto &p : tabela[idx]) if (p.first == pagina) return p.second;
    // Se nao encontrado, page fault
    return -1;
}

std::size_t TabelaHash::numBuckets() const { return bcount; }
std::size_t TabelaHash::numEntradas() const {
    std::size_t c=0; for (const auto &b : tabela) c += b.size(); return c;
}
