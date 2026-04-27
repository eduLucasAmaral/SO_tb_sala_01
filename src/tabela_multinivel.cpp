#include "tabela_multinivel.h"
#include <stdexcept>

TabelaMultinivel::TabelaMultinivel(int tamNivel1, int tamNivel2)
    : n1(tamNivel1), n2(tamNivel2), nivel1(tamNivel1, nullptr) {
    if (tamNivel1 <= 0 || tamNivel2 <= 0) throw std::invalid_argument("tamanhos invalidos");
}

TabelaMultinivel::~TabelaMultinivel() {
    // Liberar todas as tabelas de segundo nivel alocadas
    for (auto p : nivel1) if (p) delete p;
}

void TabelaMultinivel::mapear(int pagina, int frame) {
    // Dividir o numero da pagina em indices de nivel1 e nivel2
    int i1 = pagina / n2; // indice do primeiro nivel
    int i2 = pagina % n2; // indice do segundo nivel
    // Se fora do espaco simulado, ignorar
    if (i1 < 0 || i1 >= n1) return;
    // Alocar a tabela do segundo nivel sob demanda
    if (!nivel1[i1]) nivel1[i1] = new std::unordered_map<int,int>();
    // Armazenar o mapeamento (entrada do segundo nivel)
    (*nivel1[i1])[i2] = frame;
}

int TabelaMultinivel::buscar(int pagina) const {
    // Calcular indices de niveis
    int i1 = pagina / n2;
    int i2 = pagina % n2;
    // Validar indice do primeiro nivel
    if (i1 < 0 || i1 >= n1) return -1;
    // Se a tabela do segundo nivel nao existe, entao page fault
    auto tbl = nivel1[i1];
    if (!tbl) return -1;
    // Procurar a entrada no segundo nivel
    auto it = tbl->find(i2);
    if (it == tbl->end()) return -1;
    return it->second; // retornar frame mapeado
}

std::size_t TabelaMultinivel::usoMemoria() const {
    std::size_t cnt = 0;
    for (auto p : nivel1) if (p) ++cnt;
    return cnt;
}
