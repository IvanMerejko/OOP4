//
// Created by ivan on 10.05.19.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "TextClassifier.h"



long int findPositionOfNGramInTextClassifier(const typename TextClassifier::frequency_map& NGrams, const std::string& currentNGram ){
    for(int i = 0 ; i < NGrams.size() ; ++i){
        if(NGrams[i].getName() == currentNGram){
            return i;
        }
    }
    return MAX_SCORE;
}
//std::istream& getLine(std::istream& stream,
//                      std::string& string,
//                      )
bool isNoValidLetter(char letter) {
    return letter == ' ' || letter == ',' || letter == '\n' || letter == -1 || letter == '\t' || letter == '-' || letter == '{' || letter == '}'
           || letter == '.' || letter == ',' || letter == '[' || letter == ']' || letter == '!' || letter == '(' || letter == ')'
           || letter == ':' || letter == '"' || letter == '\'' || std::isdigit(letter);
}
TextClassifier::Context::Context(std::string string, unsigned long value)
        :m_string{std::move(string)} , m_value{value}{}

unsigned long TextClassifier::Context::getValue() const {
    return m_value;
}

std::string TextClassifier::Context::getName() const {
    return m_string;
}
void TextClassifier::Context::incValue() {
    ++m_value;
}


void TextClassifier::setCategory(const std::string &category) {
    m_category = category;
}

std::string TextClassifier::getCategoty()  {
    return m_category;
}

typename TextClassifier::frequency_map TextClassifier::getFrequencyMap()   {
    return m_categoryWithNGramsTable[m_category];
}
void TextClassifier::learn(std::istream &in) {
    std::string line;
    while(std::getline(in , line)){

//        std::cout << line << '\n';
        if(!line.empty()){

            createMap(line);
        }

    }
}
void TextClassifier::createMap(const std::string& text) {
    vectorOfStrings words;
    vectorOfStrings wordsForParsing;
    createVectorOfWordsFromString(text , words);
    for(auto & it : words){
        it.erase(std::remove_if(it.begin() , it.end() , isNoValidLetter), it.end());
    }
    for(int i = 2 ; i <= 5; ++i ){
        for(auto& word : words){
            createWorkingWord(i , word , wordsForParsing);
        }
        for(auto& word : wordsForParsing){
            createNGramForWord(i , word);
        }
        wordsForParsing.clear();
    }
    sort();
    deleteUnusedNGrams();
//    for(const auto& it : m_categoryWithNGramsTable[m_category]){
//        std::cout << it.getName() << "  " << it.getValue() << '\n';
//    }

}
void TextClassifier::createWorkingWord(int n, const std::string &word , vectorOfStrings& result) {
    std::string workWord{};
    workWord += '_';
    workWord += word;
    for(int i = 0 ; i < n -1 ; ++i) workWord += '_';
    result.emplace_back(workWord) ;
}
void TextClassifier::createVectorOfWordsFromString(const std::string &string, vectorOfStrings &vector) {
    std::istringstream inputStringStream(string);
    std::string tmp;
    while (inputStringStream >> tmp){
        vector.push_back(tmp);
    }
}
void TextClassifier::createNGramForWord(int n, const std::string &word) {
    for(int i = 0 ; i + n <= word.size() ; ++i){
        std::string tmp = word.substr(i , n);

        auto iteratorForThisNGramInVector = std::find_if(m_categoryWithNGramsTable[m_category].begin() , m_categoryWithNGramsTable[m_category].end() , [&tmp](const Context& NGram){
            return NGram.getName() == tmp;
        });
        if(iteratorForThisNGramInVector == m_categoryWithNGramsTable[m_category].end()){
            m_categoryWithNGramsTable[m_category].push_back(Context{tmp , 1});
        } else {
            iteratorForThisNGramInVector->incValue();
        }

    }

}
void TextClassifier::sort() {
    std::sort(m_categoryWithNGramsTable[m_category].begin() , m_categoryWithNGramsTable[m_category].end() , [](const Context& lft , const Context& rht){
        return lft.getValue() > rht.getValue();
    });
}
void TextClassifier::deleteUnusedNGrams() {
    if(m_categoryWithNGramsTable[m_category].size() > MAX_SIZE){
        m_categoryWithNGramsTable[m_category].erase(m_categoryWithNGramsTable[m_category].begin() + MAX_SIZE , m_categoryWithNGramsTable[m_category].end());
    }
}
void TextClassifier::learnFromDump(std::istream &in) {
    std::string line;
    vectorOfStrings words;
    while(std::getline(in , line)){

        createVectorOfWordsFromString(line , words);
        switch (words.front().front()){
            case 'c':
                break;
            case 's':
                m_category = words.back();
                break;
            case 'n':
                m_categoryWithNGramsTable[m_category].push_back(Context{words[1] , std::stoul(words.back())});
        }
        words.clear();

    }
//    for(const auto& it : m_categoryWithNGramsTable){
//        std::cout << it.first << '\n';
//        for(const auto& i : it.second){
//            std::cout << i.getName() << "   " << i.getValue() << '\n';
//        }
//    }
}
void TextClassifier::dumpProfile(std::ostream &out) {
    for(const auto& it : m_categoryWithNGramsTable){
        out << "s:\t" << it.first << '\n';
        for(const auto& NGram : it.second){
            out << "n:\t" << NGram.getName() << '\t' << NGram.getValue() << '\n';
        }
    }

}

long int TextClassifier::score(const TextClassifier &text) {
    auto currentCategory = m_categoryWithNGramsTable[m_category];
    long int result = 0;
    for(int i = 0 ; i < currentCategory.size() ; ++i){
        int score = findPositionOfNGramInTextClassifier(const_cast<TextClassifier &>(text).getFrequencyMap() , currentCategory[i].getName());
        result += i < score ? score - i : i - score;
    }
    return result;
}