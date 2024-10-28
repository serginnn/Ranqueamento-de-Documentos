#include "Ranqueamento.hpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
    unordered_set<string> stopwords;
    carregarStopwords("stopwords.txt", stopwords);

    unordered_map<string, int> freqGlobal;
    vector<Documento> documentos;

    vector<string> arquivos = {
        "dataset/A_mão_e_a_luva.txt", "dataset/biblia.txt", "dataset/DomCasmurro.txt",
        "dataset/quincas_borba.txt", "dataset/Semana_Machado_Assis.txt", "dataset/terremoto.txt"
    };

    for (const auto& nome : arquivos) {
        Documento doc(nome);
        processarDocumento(nome, doc, freqGlobal, stopwords);
        documentos.push_back(doc);
    }

    calcularIDF(documentos, freqGlobal);

    string frasePesquisa;
    cout << "Digite uma frase de pesquisa: ";
    getline(cin, frasePesquisa);

    vector<string> termosPesquisa;
    istringstream stream(frasePesquisa);
    string palavra;

    while (stream >> palavra) {
        removerPontuacao(palavra);
        transform(palavra.begin(), palavra.end(), palavra.begin(), ::tolower);
        if (!isStopword(palavra, stopwords)) {
            termosPesquisa.push_back(palavra);
        }
    }

    for (auto& doc : documentos) {
        calcularRelevancia(doc, termosPesquisa);
    }

    ordenarDocumentos(documentos);

    cout << "Ranking dos documentos:\n";
    for (const auto& doc : documentos) {
        cout << doc.nome << " - Relevância: " << doc.relevancia << "\n";
    }

    return 0;
}