#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <iomanip>
#include "tabela_multinivel.h"
#include "tabela_hash.h"
#include "tabela_invertida.h"

using namespace std;
using clk = chrono::high_resolution_clock;

int main() {
    cout << "Emulador de Tabelas de Paginas - Teste Basico\n";

    // Parâmetros de teste (ajustáveis)
    const int TOTAL_PAGES = 10000; // espaço lógico simulado
    const int FRAMES = 4096; // número de frames na memória física

    // Inicializar as três estruturas de páginas (nomes em português)
    TabelaMultinivel tm(100, 100); // 100 x 100 = 10k páginas possíveis
    TabelaHash th(1024); // número de buckets para a tabela hash
    TabelaInvertida ti(FRAMES); // tabela invertida com 'FRAMES' entradas

    // Preparar mapeamentos: mapear um conjunto de páginas esparsas
    vector<int> paginas_mapeadas;
    std::mt19937 rng(12345); // gerador determinístico para reprodutibilidade
    std::uniform_int_distribution<int> dist(0, TOTAL_PAGES-1);
    for (int i=0;i<2000;i++) {
        int p = dist(rng);      // escolher página aleatória no espaço lógico
        int f = i % FRAMES;     // atribuir um frame (reutilização simples)
        paginas_mapeadas.push_back(p);
        tm.mapear(p, f);        // mapear na tabela multinível
        th.mapear(p, f);        // mapear na tabela hash
        ti.mapear(p, f);        // mapear na tabela invertida
    }

    // Construir consultas: misturar consultas válidas e inválidas (page faults)
    vector<int> consultas;
    for (int i=0;i<1500;i++) consultas.push_back(paginas_mapeadas[i]);
    for (int i=0;i<500;i++) consultas.push_back(TOTAL_PAGES + i); // páginas fora do espaço -> page fault

    // Função lambda para medir tempo total de múltiplas buscas e contar faults
    auto medir = [&](auto &tabela, auto funcBuscar) {
        auto t0 = clk::now();
        int faults = 0;
        for (int p : consultas) {
            int f = funcBuscar(tabela, p);
            if (f == -1) ++faults; // contar page fault
        }
        auto t1 = clk::now();
        double ms = chrono::duration<double, milli>(t1-t0).count();
        return pair<double,int>(ms, faults);
    };

    // Medir cada estrutura e imprimir resultados (tempo total e faults)
    auto res_tm = medir(tm, [](TabelaMultinivel &t, int p){ return t.buscar(p); });
    cout << fixed << setprecision(3);
    cout << "Multinivel: tempo(ms)=" << res_tm.first << " faults=" << res_tm.second << " memoria(tabelas segundo nivel)=" << tm.usoMemoria() << "\n";

    auto res_th = medir(th, [](TabelaHash &t, int p){ return t.buscar(p); });
    cout << "Hash:      tempo(ms)=" << res_th.first << " faults=" << res_th.second << " buckets=" << th.numBuckets() << " entradas=" << th.numEntradas() << "\n";

    auto res_ti = medir(ti, [](TabelaInvertida &t, int p){ return t.buscar(p); });
    cout << "Invertida: tempo(ms)=" << res_ti.first << " faults=" << res_ti.second << " frames=" << ti.numFrames() << "\n";

    cout << "Fim do teste basico.\n";
    return 0;
}
