//
// Created by ivan on 10.05.19.
//

#ifndef OOP4_TEXTCLASSIFIER_H
#define OOP4_TEXTCLASSIFIER_H

#include <string>
#include <vector>
#include <map>

#define MAX_SIZE 1000
#define MAX_SCORE 512
#define UNKNOWN_TEXT_CATEGORY (MAX_SIZE) * (MAX_SCORE)

class TextClassifier {
    class Context{
        std::string m_string;
        unsigned long m_value;
    public:
        Context(std::string string , unsigned long value);
        std::string getName() const ;
        unsigned long getValue() const;
        void incValue();

    };

public:
    using frequency_map = std::vector<Context>;
    using vectorOfStrings = std::vector<std::string>;
    TextClassifier() = default;

    frequency_map getFrequencyMap();
    std::string getCategoty() ;
    void setCategory(const std::string& category);


    void dumpProfile(std::ostream& out);
    void learn(std::istream& in);


    void learnFromDump(std::istream& in);
    long int score(const TextClassifier&);

private:
    void createMap(const std::string& text);
    void createWorkingWord(int n , const std::string &word , vectorOfStrings& result);
    void createVectorOfWordsFromString(const std::string& string , vectorOfStrings& vector);
    void createNGramForWord(int n , const std::string& word);
    void sort();
    void deleteUnusedNGrams();
private:
    std::map<std::string , frequency_map> m_categoryWithNGramsTable;
    std::string m_category;
};


#endif //OOP4_TEXTCLASSIFIER_H
