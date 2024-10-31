<h1 align="center">Ranqueamento de Documetos</h1>

<div align="center">
    <img align="center" height="20px" width="70px" src="https://img.shields.io/badge/c++-%2300599C.svg?logo=c%2B%2B&logoColor=white"/>
    <img align="center" height="20px" width="80px" src="https://img.shields.io/badge/VS%20Code-blue?logo=visual%20studio%20code"/>
    <img align="center" alt="CMake" src="https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black" />
</div>

##  Índice :scroll:

* [Trabalho](#trabalho)
  * [Leitura dos Documentos](#leitura-dos-documentos)
  * [Cálculo do TF/IDF e Ranqueamento](#cálculo-do-tf/idf-e-ranqueamento)
* [Análise Computacional](#análise-computacional)
  * [Estrutura de dados](#estrutura-de-dados)
  * [Operações e complexidade](#operações-e-complexidade)
  * [Funcionamento](#funcionamento)
  * [Desemepenho](#desempenho)
* [Análise Crítica](#análise-crítica)
* [Conclusão](#conclusão)
* [Compilação](#compilação)


## Trabalho
Neste trabalho, foi implementado um sistema de ranqueamento de documentos com base no algoritmo TF/IDF (do inglês, Term Frequency-Inverse Document Frequency). Utilizando conceitos da disciplina de Algoritmos e Estrutura de Dados I, como análise assintótica, métodos de ordenação e tabelas hash. Assim funcionando como uma introdução para estruturas mais avançadas, como árvores e grafos.

### Leitura dos Documentos 
A primeira parte do trabalho se da pela leitura do documento de stopwords[stopwords.txt](dataset/stopwords.txt), que é um documento que contém palavras "que podem ser ignoradas" ou seja não vai me ajudar nos cálculos para o ranqueamento. Assim em sequência 



### Cálculo do TF/IDF e Ranqueamento
Na fase de teste, o segundo arquivo, [poker-hand-testing.data](dataset/poker-hand-testing.data), é lido para avaliar a eficácia do modelo treinado. Este arquivo contém novas instâncias que precisam ser classificadas, e o processo de teste se inicia pela leitura dessas instâncias. Cada linha do arquivo de teste representa uma entrada que será convertida em Features no formato <coluna, elemento>, da mesma maneira que foi feito durante a fase de treinamento.

Após essa conversão, a última coluna de cada linha, que indica a classe verdadeira da instância, é separada para servir como referência na avaliação da precisão do modelo. O processo de classificação começa com o algoritmo LAC (Lazy Associative Classification) utilizando as estruturas de dados (map de features e vetor de classes) construídas durante o treinamento.

Para cada nova instância, o algoritmo busca correspondências nas features já conhecidas e tenta identificar padrões que permitam prever a classe correta. A classe prevista pelo modelo é então comparada com a classe real da instância. Os resultados dessa comparação são registrados para determinar a precisão do modelo: acertos aumentam a contagem de acurácia, enquanto erros aumentam a contagem de perdas.

Ao final do processo, o número da linha e a classe atribuída pelo modelo são gravados no arquivo de saída output.txt. Este arquivo serve como um log detalhado da fase de teste, permitindo a análise dos resultados.


##  Análise Computacional

### Estrutura de dados
    Se o código usasse uma lista (list) ou uma fila (queue) para armazenar os termos, ele teria que percorrer a estrutura inteira para contar a frequência de cada termo, o que seria ineficiente.
    O unordered_map, por outro lado, permite atualizar e acessar a frequência de cada termo diretamente com base na chave (o termo), tornando o processo muito mais rápido.




### Operações e complexidade
Nessa parte tem-se uma análise mais completa e dedicada à cada função utilizada no trabalho para o seu funcionamento, explicando seus parâmetros, próposito, funcionamento e tempo gasto previsto, sendo essas funções [LSH](#lsh), [calcularSuporte](#calcularsuporte), [Classificação](#classificação), [Testando](#testando) e [Processando](#processando).


### Funcionamento
```Markdown
bool lsh(map<double, int>* map_lsh, 
         vector<tuple<int, int>> a, 
         vector<tuple<int, int>> b,  
         int* numero_classe,
         double* jaccard) {

    vector<tuple<int, int>> uniao; 
    vector<tuple<int, int>> interseccao; 

    set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(uniao));
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(interseccao)); 

    *jaccard = static_cast<double>(interseccao.size()) / uniao.size();
    *jaccard = std::round(*jaccard * 1000.0) / 1000.0;


    if (0.7 < *jaccard && map_lsh->find(*jaccard) != map_lsh->end()) {
        *numero_classe = map_lsh->at(*jaccard);
        return true;
    } else {
        return false;
    }
}
```
Propósito: Essa função verifica se a similaridade de Jaccard entre dois conjuntos é suficiente para associar um determinado item a uma classe conhecida. Se a similaridade for acima de um determinado limiar e a classe correspondente for encontrada em um mapa, a função retorna true e define numero_classe para a classe correspondente.

Parâmetros:
- map_lsh: Mapa que relaciona a similaridade de Jaccard às classes.
- a, b: Dois vetores de tuplas representando as características dos elementos que estão sendo comparados.
- numero_classe: Ponteiro para um inteiro onde será armazenada a classe correspondente, se encontrada.
- jaccard: Ponteiro para um valor double onde será armazenada a similaridade de Jaccard calculada.

Funcionamento:
- Calcula a união e a interseção dos conjuntos a e b.
- Calcula a similaridade de Jaccard entre a e b.
- Se a similaridade for maior que 0,1 e a similaridade existir no map_lsh, a função atribui numero_classe com a classe correspondente e retorna true.


### Desempenho 
```Markdown
void calcularSuporte(
    vector<int>combinacoes, 
    const vector<vector<int>>& classes, 
    const int features_size,
    map<vector<int>, double> *result
) 
{   
    for (const auto& c : classes) {
        vector<int> intersecao;

        set_intersection(
            combinacoes.begin(), combinacoes.end(), 
            c.begin(), c.end(), 
            back_inserter(intersecao)
        );  

        int confianca = intersecao.size();
        
        if (confianca > 0) {
            double suporte = static_cast<double>(confianca) / features_size;

            if (result->find(c) == result->end()) {
                (*result)[c] = 0;
            }
            (*result)[c] += suporte;
        }
    }
}
```
Propósito: Essa função calcula o suporte de um conjunto de combinações de elementos, comparando-o com classes existentes.

Parâmetros:
- combinacoes: Vetor de inteiros representando combinações de elementos.
- classes: Vetor de vetores de inteiros que representam as classes.
- features_size: Tamanho das características.
- result: Mapa onde os resultados dos cálculos de suporte são armazenados.

Funcionamento:
- Para cada classe, calcula a interseção entre as combinações e os elementos da classe.
- Se a interseção não for vazia, calcula o suporte (a confiança) e adiciona ao resultado.




## Análise Crítica


## Conclusão 
Neste trabalho, foi implementado o algoritmo Lazy Associative Classification (LAC)


notou-se que a implementação de uma tabela hash para busca de dados foi primordial para a diminuição do tempo de execução.




## Compilação 
Para executar o programa foi utilizado um arquivo Makefile que realiza o processo de compilação e execução. Arquivo o qual requer as seguintes diretrizes de execução:

| Comando                |  Função                                                                                               |                     
| -----------------------| ------------------------------------------------------------------------------------------------------|
|  `make clean`          | Apaga a última compilação realizada contida na pasta build                                            |
|  `make`                | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build               |
|  `make run`            | Executa o programa da pasta build após a realização da compilação                                     |



