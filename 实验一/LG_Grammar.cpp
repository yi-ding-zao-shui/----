/*
 * @Description: Advanced Language and Its Grammar
 * @Author: Tan
 * @Date: 2023-03-13 14:28:48
 * @LastEditTime: 2023-03-27 21:13:32
 * @LastEditors: Tan
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class Grammar {
public:
    // G = (Vn, Vt, P, S);
    unordered_set<char> V;
    unordered_set<char> T;
    unordered_map<string, vector<string>> P;
    char S;
    /**
     * @description: Read grammar through file path
     * @param {string} filePath
     * @return {*}
     * @Author: Tan
     * @Date: 2023-03-14 10:18:19
     * @LastEditTime: Do not edit
     * @LastEditors: Tan
     */    
    Grammar(string filePath) {
        
        ifstream fin(filePath);
        if (!fin) {
            cout << "Fail to read file!" << endl;
        } else {
            string variable, terminal, productions;
            getline(fin, variable);
            getline(fin, terminal);
            getline(fin, productions);
            // Start Symbol
            S = fin.get(); 

            // Read Variables
            for (int i = 0; i < variable.size(); i += 2) {
                V.insert(variable[i]);
            }

            // Read Terminal
            for (int i = 0; i < terminal.size(); i += 2) {
                T.insert(terminal[i]);
            }

            // Read Production
            string production;
            for (int i = 0; i < productions.size(); i++) {
                char ch = productions[i];
                // Whenever a comma is encountered, 
                // it indicates that a set of production expressions is obtained
                if (ch == ',') {
                    DecomposeProduction(production);
                    production = "";
                } else {
                    production += ch;
                }
            }
            DecomposeProduction(production);
        }
    }

    /**
     * @description: Decomposition production formula
     * @param {string&} production
     * @return {*}
     * @Author: Tan
     * @Date: 2023-03-14 10:19:59
     * @LastEditTime: Do not edit
     * @LastEditors: Tan
     */
    void DecomposeProduction(string& production) {
        int pos = production.find('-');
        string left = production.substr(0, pos);
        string right = production.substr(pos + 2);
        string cur;
        for (char& ch : right) {
            if (ch == '|') {
                P[left].emplace_back(cur);
                cur = "";
            } else {
                cur += ch;
            }
        }
        P[left].emplace_back(cur);
    }
};

/**
 * @description: Check whether it is a string or not in the variable collection
 * @param {string&} s
 * @param {Grammar&} G
 * @return {*}
 * @Author: Tan
 * @Date: 2023-03-14 10:32:13
 * @LastEditTime: Do not edit
 * @LastEditors: Tan
 */
bool isOfVariable(string s, Grammar& G) {
    return s.size() == 1 && G.V.count(s[0]);
}

/**
 * @description: Check if it is CSG
 * @param {Grammar&} G
 * @return {*}
 * @Author: Tan
 * @Date: 2023-03-14 10:35:31
 * @LastEditTime: Do not edit
 * @LastEditors: Tan
 */
bool check_CSG(Grammar& G) {
    for (auto [l, r] : G.P) {
        for (string& rr : r) {
            if (rr.size() < l.size() || rr == ".") {
                return false;
            }
        }
    }
    return true;
}

/**
 * @description: Check if it is CFG
 * @param {Grammar&} G
 * @return {*}
 * @Author: Tan
 * @Date: 2023-03-14 10:22:59
 * @LastEditTime: Do not edit
 * @LastEditors: Tan
 */
bool check_CFG(Grammar& G) {
    for (auto [l, r] : G.P) {
        if (isOfVariable(l, G) == false) {
            return false;
        }
        for (string& rr : r) {
            if (rr.size() < l.size() || rr == ".") {
                return false;
            }
        }
    }
    return true;
}

/**
 * @description: Check if it is RG
 * @param {Grammar&} G
 * @return {*}
 * @Author: Tan
 * @Date: 2023-03-14 10:20:50
 * @LastEditTime: Do not edit
 * @LastEditors: Tan
 */
bool check_RG(Grammar& G) {
    /* 
    A -> w
    A -> Bw 
    */
    bool isLeft = true;
    for (auto [l, r] : G.P) {
        // Check the left side of the production
        if (isOfVariable(l, G) == false) {
            isLeft = false;
            break;
        }
        // Check the right side of the production
        for (int i = 0; i < (int)r.size() && isLeft; i++) {
            string rr = r[i];
            if (rr.size() == 1) {
                if (G.V.count(rr[0]) || G.T.count(rr[0]) == 0) {
                    isLeft = false;
                    break;
                }
            } else {
                for (int i = 1; i < (int)rr.size(); i++) {
                    if (G.T.count(rr[i]) == 0) {
                        isLeft = false;
                        break;
                    }
                }
            }
        }
    }
    if (isLeft) {
        return true;
    }

    /* 
    A -> w
    A -> wB
    */
    bool isRight = true;
    for (auto [l, r] : G.P) {
        //  Check the left side of the production
        if (isOfVariable(l, G) == false) {
            isRight = false;
            break;
        }
        //  Check the right side of the production
        for (int i = 0; i < (int)r.size() && isRight; i++) {
            string rr = r[i];
            if (rr.size() == 1) {
                if (G.V.count(rr[0]) || G.T.count(rr[0]) == 0) {
                    isRight = false;
                    break;
                }
            } else {
                for (int i = (int)rr.size() - 2; i >= 0; i--) {
                    if (G.T.count(rr[i]) == 0) {
                        isRight = false;
                        break;
                    }
                }
            }
        }
    }
    return isRight;
}

/**
 * @description: Print Grammar
 * @param {Grammar&} G
 * @return {*}
 * @Author: Tan
 * @Date: 2023-03-14 10:20:25
 * @LastEditTime: Do not edit
 * @LastEditors: Tan
 */
void printGrammar(Grammar& G) {
    // V
    cout << "The variable:" << endl; 
    for (char c : G.V) {
        cout << c << ' ';
    }
    cout << endl;

     // T
     cout << "The terminal:" << endl;
    for (char c : G.T) {
        cout << c << ' ';
    }
    cout << endl;

    // P
    cout << "The production:" << endl;
    for (auto [l, r] : G.P) {
        for (string cur : r) {
            cout << l << "->" << cur << endl;
        }
    }

    // S
    cout << "The start symbol is " << G.S << endl;
}

/**
 * @description: Check which grammar the grammar is
 * @param {Grammar&} G
 * @return {*}
 * @Author: Tan
 * @Date: 2023-03-26 22:10:38
 * @LastEditTime: Do not edit
 * @LastEditors: Tan
 */
void check_Grammar(Grammar& G) {
    if (check_RG(G)) {
        cout << "The current grammar is RG" << endl;
    } else if (check_CFG(G)) {
        cout << "The current grammar is CFG" << endl;
    } else if (check_CSG(G)) {
        cout << "The current grammar is CSG" << endl;
    } else {
        cout << "The current grammar is PSG" << endl;
    }
    cout << endl;
}

int main() {
    /* const string filePath = "./Grammar.txt";
    const string RG_filePath = "./RG.txt";
    const string CFG_filePath = "./CFG.txt";
    const string CSG_filePath = "./CSG.txt";
    const string PSL_filePath = "./PSL.txt";
    Grammar G(RG_filePath);
    printGrammar(G);
    check_Grammar(G); */

    // "./Grammar.txt", "./RG.txt", "./CFG.txt", "./CSG.txt", "./PSG.txt"
    vector<string> paths({"./Grammar.txt", "./RG.txt", "./CFG.txt", "./CSG.txt", "./PSG.txt"});
    for (string& path : paths) {
        Grammar G(path);
        printGrammar(G); // print grammar
        check_Grammar(G); // Determine the type of grammar
        cout << "---------------------------------------" << endl << endl;
    }
    system("pause");
    return 0;
}