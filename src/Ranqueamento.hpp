#ifndef RANQUEAMENTO_HPP
#define RANQUEAMENTO_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Documento {
    string nome;
    unordered_map<string, double> tf_idf;
    double relevancia = 0;
    Documento(const string& nomeDoc) : nome(nomeDoc) {}
};


void carregarStopwords(const string& nome_arquivo, unordered_set<string>& stopwords);
bool isStopword(const string& palavra, const unordered_set<string>& stopwords);
void processarDocumento(const string& nome_arquivo, Documento& doc, unordered_map<string, int>& freqGlobal, const unordered_set<string>& stopwords);
void calcularIDF(vector<Documento>& documentos, const unordered_map<string, int>& freqGlobal);
void calcularRelevancia(Documento& doc, const vector<std::string>& frasePesquisa);
void ordenarDocumentos(vector<Documento>& documentos);
void removerPontuacao(string& palavra);
void lerFrases(const string& nome_arquivo, vector<string>& frases);

#endif