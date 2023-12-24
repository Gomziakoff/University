#include <iostream>
#include "Graph.h"
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;


string convertStringToNumbers(const string& input, unordered_map<char, int>& charMap) {
    string result;

    for (char ch : input) {
        if (isalpha(ch)) {
            char convertedChar = toupper(ch);

            if (charMap.find(convertedChar) == charMap.end()) {
                // Если буква встречается первый раз, присвоить ей новый номер
                int number = charMap.size() ;
                charMap[convertedChar] = number;
                result += to_string(number);
            }
            else {
                // Если буква уже встречалась, присвоить ей тот же номер
                result += to_string(charMap[convertedChar]);
            }
        }
        else {
            result += ch;
        }
    }

    return result;
}

int main() {
    ifstream inputFile1("input1.txt");
    if (!inputFile1.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }
    ifstream inputFile2("input2.txt");
    if (!inputFile2.is_open()) {
        cerr << "Error opening input file." << endl;
        return 1;
    }
    ofstream outputFile1("input1conv.txt");
    if (!outputFile1.is_open()) {
        cerr << "Error opening output file." << endl;
        inputFile1.close();
        return 1;
    }
    ofstream outputFile2("input2conv.txt");
    if (!outputFile2.is_open()) {
        cerr << "Error opening output file." << endl;
        inputFile2.close();
        return 1;
    }
    unordered_map<char, int> charMap1;
    string line;
    while (getline(inputFile1, line)) {
        string convertedLine = convertStringToNumbers(line, charMap1);
        outputFile1 << convertedLine << endl;
    }
    unordered_map<char, int> charMap2;
    while (getline(inputFile2, line)) {
        string convertedLine = convertStringToNumbers(line, charMap2);
        outputFile2 << convertedLine << endl;
    }
    inputFile1.close();
    outputFile1.close();
    inputFile2.close();
    outputFile2.close();
    //cout << "Conversion complete." << endl;

    ifstream in1("input1conv.txt");
    ifstream in2("input2conv.txt");
    vector<int> hash1;
    vector<int> hash2;
    
        int n;
        in1 >> n;
        Graph T1(n);
        int a, b;
        while (in1 >> a >> b) {
            T1.addEdge(a, b);
        }
        T1.DFS(0);
        hash1 = T1.getHash();
        cout << "T1 tree\n";
        T1.ShowTree();
    
   
        
        in2 >> n;
        Graph T2(n);
        
        while (in2 >> a >> b) {
            T2.addEdge(a, b);
        }
        T2.DFS(0);
        hash2 = T2.getHash();
        cout << "T2 tree\n";
        T2.ShowTree();
    
    if (hash1 == hash2) {
        cout << "Изоморфны";
    }
    else {
        cout << "Неизоморфны";
    }
    cout << "Неизоморфны";
    
    return 0;
}