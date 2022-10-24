#include <iostream>
#include <cmath>
#include <string>
using namespace std;
int main()
{
    double p, q, n, l, d = 0 , e = 0,k = 0, pr[40], pr1[40] = {0};
    double word[1000] = {0};
    long long int shifr[1000]={0};
    string s;

    for (int i = 0; i < 30; i++) {
        pr[i] = i;
    }
    for (int i = 2; i < 31; i++) {
        if (pr[i] != 0) {
            pr1[i] = pr[i];
            for (int j = i * i; j < 31; j = j + i)
                pr[j] = 0;
        }
    }
    cout << "Введите слово ";
        cin >> s;
    for (int i = 0; i < s.length(); i++)
        word[i] = s[i] - 100;
    
    while (!k) {
 cout << "Ведите первое простое число: ";
            cin >> p;
        for (int i = 0; i < 30; i++)
            if (p == pr1[i]) {
                k = k + 1;
                break;
            }
    }
    k = 0;
    while (!k) {
        cout << "Ведите второе простое число: ";
        cin >> q;
        for (int i = 0; i < 30; i++)
            if (q == pr1[i]) {
                k = k + 1;
                break;
            }
    }
    k = 0;


    n = p * q;
    l = (p - 1) * (q - 1);
    while (!k) { //подбираем взаимопростое число
        d = d + 1;
        if (int(l)%int(d) > 0) {
            k = 1;
        }
    }
    k = 0;
    cout << "Открытый ключ " << d <<" "<<n << endl;
    while (!k) { //подбираем закрытый ключ
        e = e + 1;
        if (int((e * d)) % int(l) == 1) {
            k = 1;
        }
    }
    cout << "Секретный ключ " << e << endl;
    cout << "Закодированный текст ";
    for (int i = 0; i < s.length(); i++) {//шифруем
        shifr[i] =long long int(pow(word[i], e)) % long long int(n);
        s[i] = shifr[i] + 100;
        cout << s[i]<<" ";
    }
    cout << endl;
    cout << "Расшифрованный текст ";
    for (int i = 0; i < s.length(); i++) {//расшифруем
        shifr[i] = long long int(pow(shifr[i], d)) % long long int(n);
        s[i] = shifr[i] + 100;
    }

    cout << s;
}