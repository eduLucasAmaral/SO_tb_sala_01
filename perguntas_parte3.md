## 1) Paginação Hierárquica (Multinível)
- Resposta:

  A tabela multinível aloca as tabelas de segundo nível somente quando necessário. Em um processo que usa poucos blocos isolados, apenas os buckets (entradas do primeiro nível) correspondentes às páginas usadas terão uma segunda tabela alocada — as demais permanecem nulas. Assim evita-se alocar uma entrada para cada página possível (como na tabela simples), reduzindo fortemente o uso de memória quando o espaço lógico é esparso.

- Trecho de código C++ relevante:
```cpp
// trechos do arquivo src/tabela_multinivel.cpp
void TabelaMultinivel::mapear(int pagina, int frame) {
  int i1 = pagina / n2;
  int i2 = pagina % n2;
  if (!nivel1[i1]) nivel1[i1] = new std::unordered_map<int,int>();
  (*nivel1[i1])[i2] = frame;
}

int TabelaMultinivel::buscar(int pagina) const {
  int i1 = pagina / n2;
  int i2 = pagina % n2;
  auto tbl = nivel1[i1];
  if (!tbl) return -1;
  auto it = tbl->find(i2);
  return it == tbl->end() ? -1 : it->second;
}
```
- Observações dos testes (quantos frames mapeados, memória consumida pela tabela, etc.):

- Observações dos testes:
  - No teste básico do emulador foram mapeadas 2000 páginas esparsas.
  - O número de tabelas de segundo nível alocadas (i.e. entradas não-nulas do primeiro nível) foi relatado pelo programa e é bem menor que o espaço total quando os mapeamentos são esparsos.


  - Observação: o valor memoria(tabelas segundo nivel)=100 indica que 100 tabelas de segundo nível foram alocadas para suportar os 1793 mapeamentos efetivos (o programa mapeou ~1793 entradas reais nesta execução), demonstrando economia frente à tabela simples completa.




## 2) Tabela de Páginas com Hash
- Resposta:

  A busca em hash é O(1) em média, mas com chaining e uma tabela muito pequena o tempo degenera para O(n) no pior caso (quando muitas chaves caem no mesmo bucket). O SO balanceia isso escolhendo um número de buckets proporcional ao número esperado de entradas (manter fator de carga abaixo de um limite), ou redimensionando/rehash quando o fator de carga ultrapassa um limiar.

- Trecho de código C++ relevante (função hash, inserção e busca):
```cpp
// trechos do arquivo src/tabela_hash.cpp
void TabelaHash::mapear(int pagina, int frame) {
  std::hash<int> h;
  std::size_t idx = h(pagina) % bcount;
  for (auto &p : tabela[idx]) {
    if (p.first == pagina) { p.second = frame; return; }
  }
  tabela[idx].push_back({pagina, frame});
}

int TabelaHash::buscar(int pagina) const {
  std::hash<int> h;
  std::size_t idx = h(pagina) % bcount;
  for (const auto &p : tabela[idx]) if (p.first == pagina) return p.second;
  return -1;
}
```
- Observações dos testes (fator de carga, número médio de colisões por bucket, tempos médios de busca):

- Observações dos testes:
  - No teste simples usamos 1024 buckets e 2000 entradas; fator de carga ~1.95.
  - O emulador reporta o número de entradas e buckets; tempos médios de lookup ficam baixos, mas aumentam se reduzirmos buckets para << entradas (mais colisões).




## 3) Tabela de Páginas Invertida
- Resposta:

  Na implementação invertida padrão com varredura linear, a complexidade no pior caso é O(F) onde F é o número de frames (memória física). Em um SO real, a TLB (Translation Lookaside Buffer) — um cache associativo de traduções — é crucial para mascarar essa lentidão, evitando varreduras na maioria das referências de memória.

- Trecho de código C++ relevante (estrutura invertida e rotina de busca):

```cpp
// trechos do arquivo src/tabela_invertida.cpp
int TabelaInvertida::buscar(int pagina) const {
  for (std::size_t f = 0; f < inv.size(); ++f) {
    if (inv[f] == pagina) return (int)f;
  }
  return -1;
}
```
- Observações dos testes (tempo médio/pior caso para buscas, impacto em Page Faults):

- Observações dos testes:
  - Como esperado, a tabela invertida teve o maior tempo de busca total no teste básico (varredura linear sobre 4096 frames).
  - O número de Page Faults retornado foi consistente entre as implementações para as consultas fora do conjunto mapeado.




## 4) Síntese e Trade-offs
- Resposta (ranking e justificativa):
  Uso de Memória (do mais econômico para o mais custoso):
  1) Invertida — usa espaço proporcional à memória física (frames) e independe do espaço lógico.
  2) Multinível — econômico quando o espaço lógico é esparso; aloca apenas segundo níveis necessários.
  3) Tabela Simples (não implementada aqui) — precisaria de uma entrada por página lógica (mais custoso quando espaço lógico é grande).

  Velocidade de Busca (do mais rápido para o mais lento):
  1) Hash — buscas O(1) em média (quando fator de carga controlado).
  2) Multinível — duas etapas (índice de primeiro nível + segundo), desempenho consistente e rápido se segundo nível existir.
  3) Invertida — busca por varredura linear pode ser O(F) no pior caso.

- Reflexão final: 
    Não existe uma solução perfeita para todos os casos no gerenciamento de memória. Cada estrutura de tabela de páginas tem seus próprios trade-offs entre espaço e tempo. Por exemplo, a tabela invertida é eficiente em termos de espaço quando o espaço lógico é grande, mas pode ser lenta para buscas. A tabela hash é rápida para buscas, mas pode consumir mais memória se o número de buckets não for bem dimensionado. A tabela multinível é eficiente para espaços lógicos esparsos, mas pode consumir mais memória se muitos segundo níveis forem alocados. Portanto, a escolha da estrutura depende do perfil de uso do sistema e dos requisitos específicos de desempenho e memória.




## Anexos e Evidências
- Link para o repositório com o código-fonte (GitHub):
    https://github.com/eduLucasAmaral/SO_tb_sala_01

- Comandos usados para compilar e rodar os testes:
make
./emulador

## Saída do teste: 
Emulador de Tabelas de Paginas - Teste Basico
Multinivel: tempo(ms)=0.026 faults=500 memoria(tabelas segundo nivel)=100
Hash:      tempo(ms)=0.027 faults=500 buckets=1024 entradas=1793
Invertida: tempo(ms)=2.475 faults=500 frames=4096
