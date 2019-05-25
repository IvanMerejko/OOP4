#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include "TextClassifier.h"

void dumpProfiles() {
    std::ifstream English("English.txt");
    std::ifstream Spanish("Spanish.txt");
    std::ifstream France("France.txt");
    std::ifstream Italian("Italian.txt");

    if(!English.is_open() || !Spanish.is_open() || !France.is_open() || !Italian.is_open()){
        exit(1);
    }
    std::ofstream out("NGram.txt");


    TextClassifier textClassifier;

    textClassifier.setCategory("English");
    textClassifier.learn(English);


    textClassifier.setCategory("Span");
    textClassifier.learn(Spanish);


    textClassifier.setCategory("Franch");
    textClassifier.learn(France);


    textClassifier.setCategory("Itallian");
    textClassifier.learn(Italian);

    textClassifier.dumpProfile(out);
    out.close();
}
void analyzeText(std::ifstream& unknownTextFile){
    std::ifstream NGrams{"NGram.txt"};
    TextClassifier textClassifier;
    TextClassifier unknownTextClassifier;
    long int englScore = 0;
    long int spanScore = 0;
    long int franchScore = 0;
    long int itaScore = 0;


    unknownTextClassifier.learn(unknownTextFile);
    textClassifier.learnFromDump(NGrams);
    textClassifier.setCategory("English");
    std::cout << "english score :" << (englScore = textClassifier.score(unknownTextClassifier)) << '\n';
    textClassifier.setCategory("Span");
    std::cout << "Span score :" << (spanScore = textClassifier.score(unknownTextClassifier)) << '\n';
    textClassifier.setCategory("Franch");
    std::cout << "Franch score :" << (franchScore = textClassifier.score(unknownTextClassifier)) << '\n';
    textClassifier.setCategory("Itallian");
    std::cout << "Itallian score :" <<  (itaScore = textClassifier.score(unknownTextClassifier)) << '\n';

    if(englScore == spanScore && spanScore == franchScore && franchScore == itaScore){
        std::cout << "Empty file or unknown text\n";
    } else {
        auto minScore = std::min(std::min(englScore , spanScore) , std::min(franchScore , itaScore));

        if(minScore == englScore){
            std::cout << "English text\n";
        } else if(minScore == spanScore){
            std::cout << "Span text\n";
        } else if(minScore == franchScore){
            std::cout << "Franch text\n";
        } else {
            std::cout << "Itallian text\n";
        }
    }

}
int main() {
    dumpProfiles();

    std::ifstream unknownEng("unknown-English.txt");
    std::ifstream unknownFra("unknown-French.txt");
    std::ifstream unknownIta("unknown-Itallian.txt");
    std::ifstream unknownSpa("unknown-Spanish.txt");
    std::ifstream emptyFile("emptyFile.txt");


    analyzeText(unknownEng);
    std::cout << "\n\n ------------------------- \n\n";
    analyzeText(unknownFra);
    std::cout << "\n\n ------------------------- \n\n";
    analyzeText(unknownIta);
    std::cout << "\n\n ------------------------- \n\n";
    analyzeText(unknownSpa);
    std::cout << "\n\n ------------------------- \n\n";
    analyzeText(emptyFile);
    return 0;
}