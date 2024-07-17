#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <bitset>


using namespace std;


class Grammary {
private:

    set<char> nonTerminals;
    set<char> terminals;
    map<char, set<string>> grammary;
    map<string, set<char>> reverseGrammary;
    const char start = 'S';


    void setSymbols() {
        set<char> newNonTerminals;
        set<char> newTerminals;
        
        for (const auto &trans: grammary) {
            if (newNonTerminals.find(trans.first) == newNonTerminals.end()) 
                newNonTerminals.insert(trans.first);
            
            for (auto to: trans.second) {
                if (to != "_") {
                    for (auto sym: to) {
                        if (isupper(sym)) {
                            if (newNonTerminals.find(sym) == newNonTerminals.end()) 
                                newNonTerminals.insert(sym);
                        } else {
                            if (newTerminals.find(sym) == newTerminals.end()) 
                                newTerminals.insert(sym);
                        }
                    }
                }
            }
        }

        nonTerminals = newNonTerminals;
        terminals = newTerminals;
    }


    void addSymbols(string to) {
        if (to != "_") {
            for (auto sym: to) {
                if (isupper(sym)) {
                    if (nonTerminals.find(sym) == nonTerminals.end()) 
                        nonTerminals.insert(sym);
                } else {
                    if (terminals.find(sym) == terminals.end()) 
                        terminals.insert(sym);
                }
            }
        }
    }


    void setEpsNonTerminal(set<char> &epsProds) {
        bool isEnd = false;
        
        while (!isEnd) {
            isEnd = true;

            for (auto trans: grammary) {
                if (epsProds.find(trans.first) == epsProds.end()) {
                    for (string to: trans.second) {
                        bool isThere = true;

                        for (char sym: to) {
                            if (epsProds.find(sym) == epsProds.end())
                                isThere = false;
                        }

                        if (isThere) {
                            epsProds.insert(trans.first);
                            isEnd = false;
                        }
                    }
                }
            }
        }
    }


    void deleteEpsProds(const set<char> &epsProds) {
        bool isEnd = false;

        while (!isEnd) {
            isEnd = true;

            for (auto it = grammary.begin(); it != grammary.end(); it++) {
                set<string> &tos = it->second;

                for (string to: tos) {
                    for (int i = 0; i < to.length(); i++) {
                        if (epsProds.find(to[i]) != epsProds.end()) {
                            
                            string newTo = to;
                            newTo.erase(i, 1);
                            
                            if (tos.find(newTo) == tos.end()) {
                                if (!newTo.empty()) {
                                    tos.insert(newTo);
                                    isEnd = false;
                                } else {
                                    if (it->first == start) {
                                        tos.insert("_");
                                        isEnd;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    void setChainProds(map<char, set<char>> &chainProds) {
        bool isEnd = false;

        while (!isEnd) {
            isEnd = true;

            for (auto chains: chainProds) {
                set<char> froms = chains.second;
                
                for (char from: froms) {
                    for (string to: grammary[from]) {
                        if (to.length() == 1) {
                            char sym = to[0];
                            
                            if (nonTerminals.find(sym) != nonTerminals.end()) {
                                if (froms.find(sym) == froms.end()) {
                                    chainProds[chains.first].insert(sym);
                                    isEnd = false;
                                }
                            }
                        } 
                    }
                }
            }
        }
    }


    void addChainProds(const map<char, set<char>> &chainProds) {
        bool isEnd = false;

        while (!isEnd) {
            isEnd = true;
            
            map<char, set<string>> newGrammary;

            for (const auto &chains : chainProds) {
                char currentKey = chains.first;
                const set<char> &currentChain = chains.second;
                set<string> &currentTos = grammary[currentKey];

                for (char to : currentChain) {
                    const set<string> &tos = grammary[to];
                    for (const string &newTo : tos) {
                        if (currentTos.find(newTo) == currentTos.end() && newTo != "_") {
                            newGrammary[currentKey].insert(newTo);
                            isEnd = false;
                        }
                    }
                }
            }

            for (const auto &newProd : newGrammary)
                grammary[newProd.first].insert(newProd.second.begin(), newProd.second.end());
        }
    }


    void setGenerativeSymbols(set<char> &genSymbols) {
        bool isEnd = false;

        while (!isEnd) {
            isEnd = true;

            for (const auto& trans: grammary) {
                if (genSymbols.find(trans.first) == genSymbols.end()) {
                    for (string tos: trans.second) {
                        bool isGenerative = true;
                        
                        for (char sym: tos) {
                            if (genSymbols.find(sym) == genSymbols.end()) {
                                isGenerative = false;
                                break;
                            }
                        }

                        if (isGenerative) {
                            genSymbols.insert(trans.first);
                            isEnd = false;
                        }
                    }
                }
            }
        }
    }


    void setReachableSymbols(set<char> &reachSymbols) {
        bool isEnd = false;
        
        while (!isEnd) {
            isEnd = true;

            for (char from: reachSymbols) {
                for (string to: grammary[from]) {
                    for (char sym: to) {
                        if (isupper(sym) && reachSymbols.find(sym) == reachSymbols.end()) {
                            reachSymbols.insert(sym);
                            isEnd = false;
                        }
                    }
                }
            }
        }
    }


    set<char> validSymbols() {
        set<char> symbols = {};

        for (char sym = 'A'; sym <= 'Z'; sym++)
            if (grammary.count(sym) == 0) symbols.insert(sym);

        return symbols;
    }


    void decomposeTerminal() {
        set<char> symbols = validSymbols();
        map<char, set<string>> newGrammary;
        map<char, char> newTrans;

        for (const auto &trans: reverseGrammary) {
            string word = trans.first;

            if (word.length() > 1) {
                string newTo = word;

                for (int i = 0; i < word.length(); i++) {
                    if (!isupper(word[i])) {
                        if (newTrans.count(word[i]) == 0) {
                            if (!symbols.empty()) {
                                
                                newTrans[word[i]] = *symbols.begin();
                                symbols.erase(symbols.begin());
                                newTo[i] = newTrans[word[i]];

                            }
                        } else {

                            newTo[i] = newTrans[word[i]];

                        }
                    }
                }

                for (char sym: trans.second) newGrammary[sym].insert(newTo);

            } else {

                for (char sym: trans.second) newGrammary[sym].insert(word);

            }

        }

        for (auto addingTrans: newTrans) {
            string to(1, addingTrans.first);
            newGrammary[addingTrans.second] = {to};
        }

        grammary = newGrammary;

    }


    void decomposeNonTerminal() {
        set<char> symbols = validSymbols();
        map<char, set<string>> newGrammary;

        for (const auto &trans: reverseGrammary) {
            string word = trans.first;

            if (word.length() > 2) {
                char from;

                for (int i = 0; i < word.length() - 2; i++) {                                    
                    if (!symbols.empty()) {
                        string newTo = string(1, word[i]) + *symbols.begin();
                        
                        if (i == 0)
                            for (char sym: trans.second) newGrammary[sym].insert(newTo);
                        else
                            newGrammary[from].insert(newTo);

                        from = *symbols.begin();
                        symbols.erase(symbols.begin());
                    }
                }

                string newTo = string(1, word[word.length() - 2]) + word[word.length() - 1];
                newGrammary[from].insert(newTo);

            } else {

                for (char sym: trans.second) newGrammary[sym].insert(word);

            }
        }

        grammary = newGrammary;

    }


    void printCYKmatrix(const vector<vector<set<char>>>& matrix) {
        int n = matrix.size();

        cout << "CYK matrix:\n";
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << "[";
                const set<char>& cellContents = matrix[i][j];
                for (auto it = cellContents.begin(); it != cellContents.end(); ++it) {
                    cout << *it;
                    if (next(it) != cellContents.end()) cout << ",";
                }
                cout << "] ";
            }
            cout << endl;
        }
    }


    void deleteDuplicate(char save, set<char> delSymbols) {
        map<char, set<string>> newGrammary;

        for (const auto &trans: reverseGrammary) {
            string word = trans.first;
            string newTo = word;

            for (int i = 0; i < word.length(); i++) {
                if (delSymbols.find(word[i]) != delSymbols.end()) {
                    newTo[i] = save;
                }
            }

            for (char sym: trans.second) {
                if (delSymbols.find(sym) == delSymbols.end())
                    newGrammary[sym].insert(newTo);
            }

        }

        grammary = newGrammary;

    }


    void setReverseGrammaryFromGrammary() {
        map<string, set<char>> newReverseGrammary;

        for (const auto &trans: grammary) {
            for (const auto &to: trans.second) {
                newReverseGrammary[to].insert(trans.first);
            }
        }

        reverseGrammary = newReverseGrammary;

    }


public:


    Grammary(ifstream &infile) {
        char from;
        string to;

        while (infile.good()) {
            infile >> from;
            infile >> to;

            if (grammary.count(from) != 0) {
                grammary[from].insert(to);
                addSymbols(to + from);        
            } else {
                grammary[from] = {to};
                addSymbols(to + from);
            }
        }

        setReverseGrammaryFromGrammary();

    }


    void printSymbols() {
        cout << "Nonterminal symbols: ";
        for (auto sym: nonTerminals) {
            cout << sym << " ";
        }
        cout << '\n' << "Terminal symbols: ";
        for (auto sym: terminals) {
            cout << sym << " ";
        }
        cout << '\n';
    }


    void printGrammary() {
        cout << "Grammary:\n";
        for (const auto& trans: grammary) {
            cout << "    From: " << trans.first << "\n";
            int i = 1;
            for (const auto& to: trans.second) {
                cout << "\t" << i << ": " << to << "\n";
                i += 1;
            }
        }
        cout << '\n';
    }


    void deleteEps() {
        set<char> epsProds;
        
        if (reverseGrammary.count("_") != 0) {
            epsProds.insert(reverseGrammary["_"].begin(), reverseGrammary["_"].end());

            setEpsNonTerminal(epsProds);

            for (auto it = grammary.begin(); it != grammary.end(); it++) {
                if (it->first != start) {
                    auto& tos = it->second;
                    auto findIter = tos.find("_");
                    
                    if (findIter != tos.end()) tos.erase(findIter);
                }
            }

            deleteEpsProds(epsProds);

        }
    }


    void deleteChainProds() {
        map<char, set<char>> chainProds;
        for (char from: nonTerminals) chainProds[from] = {from};

        setChainProds(chainProds);

        for (auto it = grammary.begin(); it != grammary.end(); it++) {
            set<string>& tos = it->second;
            
            for (auto itTo = tos.begin(); itTo != tos.end();) {
                if ((*itTo).length() == 1) {
                    char to = (*itTo)[0];

                    if (nonTerminals.find(to) != nonTerminals.end()) {
                        itTo = tos.erase(itTo);
                    } else ++itTo;
                } else ++itTo;
            }
        }

        addChainProds(chainProds);

    }


    void generativeSymbols() {
        map<char, set<string>> newGrammary;
        set<char> genSymbols = terminals;
        genSymbols.insert('_');

        setGenerativeSymbols(genSymbols);

        for (auto trans: grammary) {
            char from = trans.first;

            if (genSymbols.find(from) != genSymbols.end()) {
                for (string to: trans.second) {
                    bool isGood = true;

                    for (char sym: to) {
                        if (genSymbols.find(sym) == genSymbols.end()) {
                            isGood = false;
                            break;
                        }
                    }

                    if (isGood) {
                        if (newGrammary.count(from) != 0) newGrammary[from].insert(to);
                        else newGrammary[from] = {to};
                    }
                }
            }
        }

        grammary = newGrammary;
        setSymbols();

    }


    void reachableSymbols() {
        map<char, set<string>> newGrammary;
        set<char> reachSymbols = {start};

        setReachableSymbols(reachSymbols);

        for (auto it = grammary.begin(); it != grammary.end(); it++) {
            char sym = it->first;

            if (reachSymbols.find(sym) != reachSymbols.end()) {
                newGrammary[sym] = it->second;
            }
        }

        grammary = newGrammary;
        setSymbols();
        setReverseGrammaryFromGrammary();

    }


    void decompose() {

        decomposeTerminal();
        setReverseGrammaryFromGrammary();
        decomposeNonTerminal();
        setReverseGrammaryFromGrammary();
        setSymbols();

    }


    void deleteSimilarTrans() {
        map<set<string>, set<char>> revGrammary;
        vector<set<char>> duplicates;
        
        for (const auto &trans: grammary) revGrammary[trans.second].insert(trans.first);
        for (const auto &trans: revGrammary)
            if (trans.second.size() > 1) duplicates.push_back(trans.second);

        for (auto duplicate: duplicates) {
            if (duplicate.find(start) == duplicate.end()) {
                char save = *duplicate.begin();
                duplicate.erase(duplicate.begin());
                deleteDuplicate(save, duplicate);
            } else {
                duplicate.erase(duplicate.find(start));
                deleteDuplicate(start, duplicate);
            } 
        }

        setReverseGrammaryFromGrammary();

    }

    bool CYK(const string& word) {
        int n = word.length();
        const int MAX_SYMBOLS = 26;
        bitset<MAX_SYMBOLS> matrix[n][n];

        for (int i = 0; i < n; i++) {
            const string& to = word.substr(i, 1);
            auto it = reverseGrammary.find(to);
            
            if (it != reverseGrammary.end()) {
                for (char sym : it->second) {
                    matrix[n - 1][i].set(sym - 'A');
                }
            }
        }

        for (int i = n - 2; i >= 0; i--) {
            for (int j = 0; j <= i; j++) {
                int count = n - i - 1;
                int k1 = n - 1, l1 = j;
                int k2 = i + 1, l2 = j + 1;

                while (count > 0) {
                    const bitset<MAX_SYMBOLS>& set1 = matrix[k1][l1];
                    const bitset<MAX_SYMBOLS>& set2 = matrix[k2][l2];

                    if (!set1.none() && !set2.none()) {
                        for (int sym1 = 0; sym1 < MAX_SYMBOLS; sym1++) {
                            if (set1.test(sym1)) {
                                for (int sym2 = 0; sym2 < MAX_SYMBOLS; sym2++) {
                                    if (set2.test(sym2)) {
                                        string to = string(1, char(sym1 + 'A')) + char(sym2 + 'A');
                                        auto it = reverseGrammary.find(to);

                                        if (it != reverseGrammary.end()) {
                                            for (char sym : it->second) {
                                                matrix[i][j].set(sym - 'A');
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    count -= 1;
                    k1 -= 1;
                    k2 += 1;
                    l2 += 1;
                }


            }
        }

        return matrix[0][0].test(start - 'A');

    }

};


int main() {
    ifstream infile("cyk.in");
    ofstream outfile("cyk.out");

    
    // READ FILE AND CREATE GRAMMARY //
    string word;
    infile >> word;

    Grammary grammary(infile);
    //-------------------------------//


    // BRINGING CHOMSKY BACK TO NORMAL FORM //
    grammary.deleteEps();
    grammary.deleteChainProds();
    grammary.generativeSymbols();
    grammary.reachableSymbols();
    grammary.decompose();
    grammary.deleteSimilarTrans();
    //--------------------------------------//


    // CHECKING WHETHER A WORD BELONGS TO GRAMMAR //
    bool isGrammarWord = grammary.CYK(word);
    if (isGrammarWord) {
        cout << "Answer: YES" << '\n';
        outfile << "YES";
    } else {
        cout << "Answer: NO" << '\n';
        outfile << "NO";
    }
    //--------------------------------------------//


    return 0;
}