#include <iostream>
#include <string>
#include <cstdlib>  
#include <windows.h>
#include <iomanip>
#include <algorithm>
using namespace std;

int main()
{
    double sum = 0;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    string str;
    getline(cin,str);
    int a = str.length();
    int* count_symb = new int[str.length()]{};
    for (int i = 0; i < str.length(); i++) {
        count_symb[i] = 1;
    }
    transform(str.begin(), str.end(), str.begin(), tolower);
    for (int i = 0; i < str.length() - 1; i++) {
        for (int j = i+1; j < str.length(); j++) {
            if (str[j] == str[i]) {
                count_symb[i]++;
                str.erase(j, 1);
                j--;
                
            }
        }
    }

    double* entrophy = new double[str.length()];
    for (int i = 0; i < str.length(); i++) {
        entrophy[i] = -1 * ((double)count_symb[i] / a) * (log((double)count_symb[i] / a) / log(2));
        sum = sum + entrophy[i];
    }

    for (int i = 0; i < str.length(); i++) {
        cout << setw(30) << left << str[i] << setw(30) << left << entrophy[i] << endl;
    }
   
    cout<< endl << sum;
}
