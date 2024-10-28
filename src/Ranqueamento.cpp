#include "Ranqueamento.hpp"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <cctype>

using namespace std;

void carregarStopwords(const string& nome_arquivo, unordered_set<string>& stopwords) {
    ifstream arquivo(nome_arquivo);
    string palavra;
    while (arquivo >> palavra) {
        stopwords.insert(palavra);
    }
}

bool isStopword(const string& palavra, const unordered_set<string>& stopwords) {
    return stopwords.count(palavra) > 0;
}

void removerPontuacao(string& palavra) {
    palavra.erase(remove_if(palavra.begin(), palavra.end(), ::ispunct), palavra.end());
}

void processarDocumento(const string& nome_arquivo, Documento& doc, unordered_map<string, int>& freqGlobal, const unordered_set<string>& stopwords) {
    ifstream arquivo(nome_arquivo);
    string palavra;
    unordered_map<string, int> freqTermos;

    // Contagem de termos no documento
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
        freqGlobal[par.first]++; // Atualiza a frequência global (DF)
    }

    doc.tf_idf.clear(); // Limpa o mapa tf_idf do documento

    // Calcula TF e armazena no documento
    for (const auto& par : freqTermos) {
        double TF = static_cast<double>(par.second) / totalTermos;
        doc.tf_idf[par.first] = TF; // Apenas o TF neste estágio
    }
}

void calcularIDF(vector<Documento>& documentos, const unordered_map<string, int>& freqGlobal) {
    int totalDocumentos = documentos.size();

    // Ajuste de IDF para cada documento
    for (auto& doc : documentos) {
        for (auto& par : doc.tf_idf) {
            const string& termo = par.first;
            double TF = par.second;
            int Df = freqGlobal.at(termo);

            // Calcula o IDF de acordo com a fórmula do método fornecido
            double IdF = log(static_cast<double>(1 + totalDocumentos) / (1 + Df));

            // Atualiza TF-IDF
            doc.tf_idf[termo] = TF * IdF;
        }
    }
}

void calcularRelevancia(Documento& doc, const vector<string>& frasePesquisa) {
    doc.relevancia = 0;
    for (const auto& termo : frasePesquisa) {
        if (doc.tf_idf.count(termo)) {
            doc.relevancia += doc.tf_idf[termo];
        }
    }
}

void ordenarDocumentos(vector<Documento>& documentos) {
    sort(documentos.begin(), documentos.end(), [](const Documento& a, const Documento& b) {
        return a.relevancia > b.relevancia;
    });
}