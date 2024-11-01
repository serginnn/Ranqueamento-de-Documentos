<h1 align="center">Ranqueamento de Documetos</h1>

<div align="center">
    <img align="center" height="20px" width="70px" src="https://img.shields.io/badge/c++-%2300599C.svg?logo=c%2B%2B&logoColor=white"/>
    <img align="center" height="20px" width="80px" src="https://img.shields.io/badge/VS%20Code-blue?logo=visual%20studio%20code"/>
    <img align="center" alt="CMake" src="https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black" />
</div>

##  Índice :scroll:

* [Trabalho](#trabalho)
  * [Cálculo do TF/IDF e Ranqueamento](#cálculo-do-tf/idf-e-ranqueamento)
* [Análise Computacional](#análise-computacional)
  * [Estrutura de dados](#estrutura-de-dados)
  * [Operações e complexidade](#operações-e-complexidade)
  * [Funcionamento](#funcionamento)
  * [Compilação](#compilação)
* [Análise Crítica](#análise-crítica)
* [Considerações Finais](#considerações-finais)


## Trabalho
Neste trabalho, foi implementado um sistema de ranqueamento de documentos com base no algoritmo TF/IDF (do inglês, Term Frequency-Inverse Document Frequency). Utilizando conceitos da disciplina de Algoritmos e Estrutura de Dados I, como análise assintótica, métodos de ordenação e tabelas hash. 

Para isso foi utilizado como base os documentos: [A_mão_e_a_luva.txt](dataset/A_mão_e_a_luva.txt), [biblia.txt](dataset/biblia.txt), [DomCasmurro.txt](dataset/DomCasmurro.txt), [quincas_borba.txt](dataset/quincas_borba.txt), [Semana_Machado_Assis.txt](dataset/Semana_Machado_Assis.txt) e [terremoto.txt](dataset/terremoto.txt). E para melhora do ranqueamento e da performance do algoritmo foi utilizado o documento de stopwords[stopwords.txt](dataset/stopwords.txt), que contém palavras que em uma busca podem ser consideradas irrelevantes e não vai ajudar nos cálculos para o ranqueamento.


### Cálculo do TF/IDF e Ranqueamento
O cálculo de TF-IDF (Term Frequency-Inverse Document Frequency) é uma técnica usada para medir a importância de um termo em um documento em relação a um conjunto de documentos (corpus). O objetivo do TF-IDF é destacar termos que são relevantes em um documento específico, mas que não são comuns em todos os documentos, ajudando a identificar termos significativos e relevantes para a busca.

Assim o Term Frequency(TF) mede a frequência com que um termo aparece no documento, sendo cálculo realizado pelo numero de recorrências do termo no documento dividido pela quantidade de termos no documento.

O Inverse Document Frequency (IDF) mede a raridade de um termo em todo o conjunto de documentos. A ideia é que termos que aparecem em muitos documentos sejam menos informativos, então o IDF reduz a importância de termos comuns. Sendo cálculado pela fórmula: $\log((1+totalDocumento)/(1+numeroDocumentoComTermos))$.

Para finalizar é realizada uma multiplicação entre os dois resultados obtidos $\ TF*IDF$, para obter um número que representará a relevância da frase em relação aos documentos do banco de dados.
##  Análise Computacional

### Estrutura de dados
Para a realização desse algoritmo foi utlilizado duas tabelas hash `unordered_map` e `unordered_set` para acesso rápido e eficiente dos termos e das stopwords, respectivamente.

O unordered_map é uma estrutura de dados que armazena pares de chave-valor e o unordered_set é uma estrutura que armazena elementos únicos e permite operações de busca, assim como o unordered_map, mas sem valores associados às chaves.

Além disso foi utilizado `vector` para guarda os documentos em uma lista e ordena-los de acordo com a relevância com o cálculo do TF/IDF. Ele foi utilizado também para armazenar os termos importantes das frases de pesquisa do arquivo para ser comparado com os documentos.

Por ultimo também foi utilizado uma `struct Documento` para armazenar informações sobre cada documento como o nome, uma mapa com o valor TF/IDF de cada termo do documento e um double que guarda o resultado final da relevância do documento para a frase de pesquisa.


### Operações e complexidade
**1. carregarStopwords**
```Markdown
void carregarStopwords(const string& nome_arquivo, unordered_set<string>& stopwords) {
    ifstream arquivo(nome_arquivo);
    string palavra;
    while (arquivo >> palavra) {
        stopwords.insert(palavra);
    }
}
```
Nessa operação é realiza a leitura do arquivo das stopwords e as palavras são adicionadas na hash. Seu custo computacional é $\ O(n) $, dependendo apenas da quantidade de stopwords.

**2. isStopword**
```Markdown
bool isStopword(const string& palavra, const unordered_set<string>& stopwords) {
    return stopwords.count(palavra) > 0;
}
```
A operação verifica se uma palavra é stopword. Apresentando custo $\ O(1) $ que remete ao custo do acesso a `unordered_set`.

**3. removerPontuacao**
```Markdown
void removerPontuacao(string& palavra) {
    palavra.erase(remove_if(palavra.begin(), palavra.end(), ::ispunct), palavra.end());
}
```
Nessa parte do código é realizada a remoção da pontuação, pois a pontuação não apresenta importância na pesquisa. Para tal utiliza-se `remove_if` para localizar os caracteres de pontuação e `erase` para remove-los, não apresentando custo expressivo.

**4. processarDocumento**
```Markdown
void processarDocumento(const string& nome_arquivo, Documento& doc, unordered_map<string, int>& freqGlobal, const unordered_set<string>& stopwords) {
    ifstream arquivo(nome_arquivo);
    string palavra;
    unordered_map<string, int> freqTermos;

    while (arquivo >> palavra) {
        removerPontuacao(palavra);
        transform(palavra.begin(), palavra.end(), palavra.begin(), ::tolower);

        if (!isStopword(palavra, stopwords) && !palavra.empty()) {
            freqTermos[palavra]++;
        }
    }

    int totalTermos = 0;
    for (const auto& par : freqTermos) {
        totalTermos += par.second;
        freqGlobal[par.first]++; 
    }

    doc.tf_idf.clear(); 

    for (const auto& par : freqTermos) {
        double TF = static_cast<double>(par.second) / totalTermos;
        doc.tf_idf[par.first] = TF; 
    }
}
```
Nesse método é realizada a leitura dos arquivos, transformação da letra para minúscula, contagem da frequência de termos e o cálculo do TF. Apresentando complexidade $\ O(n) $, sendo "n" a quantidade de palavras.

**5. calcularIDF**
```Markdown
void calcularIDF(vector<Documento>& documentos, const unordered_map<string, int>& freqGlobal) {
    int totalDocumentos = documentos.size();

    for (auto& doc : documentos) {
        for (auto& par : doc.tf_idf) {
            const string& termo = par.first;
            double TF = par.second;
            int Df = freqGlobal.at(termo);

            double IdF = log(static_cast<double>(1 + totalDocumentos) / (1 + Df));

            doc.tf_idf[termo] = TF * IdF;
        }
    }
}
```
A função percorre cada termo em cada documento, calcula o IDF e multiplica pelo TF para obter o TF-IDF. Assim apresentando uma complexidade de $\ O(n *q) $, sendo "q" a quantidade de documentos.

**6. calcularRelevancia**
```Markdown
void calcularRelevancia(Documento& doc, const vector<string>& frasePesquisa) {
    doc.relevancia = 0;
    for (const auto& termo : frasePesquisa) {
        if (doc.tf_idf.count(termo)) {
            doc.relevancia += doc.tf_idf[termo];
        }
    }
}
```
A função calcula a relevância de cada documento para uma frase de pesquisa, somando o TF-IDF de cada termo da frase que aparece no documento. Dessa forma analisando assíntoticamente o custo computacional é $\ O(n) $, uma vez que a busca na hash tem custo $\ O(1) $ e o gasto esta em percorrer a frse de pesquisa.

**7. Ordenação**
```Markdown
void ordenarDocumentos(vector<Documento>& documentos) {
    sort(documentos.begin(), documentos.end(), [](const Documento& a, const Documento& b) {
        return a.relevancia > b.relevancia;
    });
}
```
Para ordenar os documentos de acordo com os resultados obtidos a partir da frase de pesquisa, foi utilizado o `std::sort`.

O `std::sort` usa o Introsort, que combina o Quicksort, Heapsort, e Insertion Sort. O Introsort começa com o Quicksort, mas monitora a profundidade da recursão. Se ela ultrapassar um limite, o algoritmo muda para o Heapsort para evitar o pior caso do Quicksort. No final, o Insertion Sort é usado em pequenos segmentos para otimizar ainda mais a ordenação. Tendo um custo $\ O(nlogn)$, o que a torna eficiente para a maioria dos casos.

- **Conclusão**

Pode-se definir então que o custo médio do algoritmo no geral é de $\ O(n) $ considerando $\ n $ o número total de palavras em todos os documentos. Uma vez que o maior custo apresentado em uma função/operação acima foi de $\ O(n) $.

### Funcionamento

* **Exemplo de entrada**:
```Markdown
Deus, seja louvado!
Amanha vou tomar sorvete
que dia lindo
Madrid dia
```
Cada frase deve estar em uma linha do arquivo [frases.txt](dataset/frases.txt).

* **Exemplo de saída**: 
```Markdown
Ranking dos documentos para a frase: "Deus, seja louvado!"
dataset/biblia.txt - Relevância: 0.00105657
dataset/DomCasmurro.txt - Relevância: 0.000236076
dataset/Semana_Machado_Assis.txt - Relevância: 0.000198816
dataset/quincas_borba.txt - Relevância: 0.000127848
dataset/A_mão_e_a_luva.txt - Relevância: 9.52416e-05
dataset/terremoto.txt - Relevância: 0
```
A saída é ordenada com base no resultado obtido dos cálculos e para cada frase uma saída nova é apresentada no terminal.

### Compilação 
Para executar o programa foi utilizado um arquivo Makefile que realiza o processo de compilação e execução. Arquivo o qual requer as seguintes diretrizes de execução:

| Comando                |  Função                                                                                               |                     
| -----------------------| ------------------------------------------------------------------------------------------------------|
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                            |
|  `make`                | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build               |
|  `make run`            | Executa o programa da pasta build após a realização da compilação                                     |

## Análise Crítica
**1. Uso de Árvores de Busca (como AVL ou BTS) para Stopwords e Frequências de Termos**:

Em vez de um `unordered_set` para as stopwords, uma estrutura de árvore balanceada (como uma árvore AVL ou BTS) poderia ser usada para armazenar as stopwords ou o índice global de frequência dos termos. 

Isso permite buscas eficientes com uma complexidade $\ O(log n)$ e uma melhor organização de dados quando há uma grande quantidade de termos ou stopwords. Tendo em vista que os dados são guardados ordenadamente, tornando possível realizar busca por prefixos ou intervalos.

Podendo ser mais eficiente para grandes conjuntos de dados onde a busca $\ O(log n)$ é preferível a buscas em média $\ O(1)$ (como em hash tables), especialmente se o balanceamento é importante para a estrutura. 

Entretanto estruturas como árvores possuem uma maior complexidade, uma vez que precisam de operações adicionais de balanceamento durante a inserção e remoção de elementos. Além de consumirem mais memória.


**2. Uso de Grafos entre Documentos**:

Os documentos podem ser representados como nós em um grafo, com arestas que conectam documentos "similares" (com base em métricas de similaridade, como cosseno ou Jaccard, em função dos termos ou do TF-IDF). Dessa forma, os documentos mais semelhantes ficam próximos uns dos outros.

Facilitando a recomendação de documentos relacionados. Por exemplo, ao calcular a relevância de um documento específico para uma frase de pesquisa, o sistema também poderia sugerir documentos vizinhos no grafo que têm uma alta similaridade.


## Considerações Finais
Nota-se que estruturas como `unordered_map` e `vector` apresentam bom desempenho para esse trabalho. O uso de estrutura mais avançadas como arvores e grafos apesar de apresentarem custo médio $\ O(logn) $ até para conjuntos de dados muito grandes, requerem um custo de memória maior e uma maior implementação. 

Assim sendo para esse problema as estruturas utilizadas apesar de mais simples, exercem uma boa eficiência no geral. Com valores melhores para operações de simples inserções e busca de termos individuais.

Entretanto para trabalhos maiores e mais complexos, a busca com métodos ordenados presente nos algoritmos de arvores e grafos ajudam na busca e manutenção, caso necessário. Além do tempo que pravalecerá melhor nas estruturas mais complexas.







