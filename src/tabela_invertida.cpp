#include "tabela_invertida.h"
#include <stdexcept>

TabelaInvertida::TabelaInvertida(std::size_t frames)
    : inv(frames, -1) {
    if (frames == 0) throw std::invalid_argument("frames deve ser > 0");
}

void TabelaInvertida::mapear(int pagina, int frame) {
    // Se o frame estiver fora do intervalo, ignorar
    if (frame < 0 || (std::size_t)frame >= inv.size()) return;
    // Armazenar no vetor invertido: inv[frame] = pagina
    inv[frame] = pagina;
}

int TabelaInvertida::buscar(int pagina) const {
    // Varredura linear sobre todos os frames: pior caso O(N)
    for (std::size_t f = 0; f < inv.size(); ++f) {
        if (inv[f] == pagina) return (int)f; // encontrado
    }
    // Nao encontrado -> page fault
    return -1;
}

std::size_t TabelaInvertida::numFrames() const { return inv.size(); }
